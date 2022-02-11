#include "set.h"

/***********************************************
 * CONSTRUCTOR
 * *********************************************/

 /************************************************
  * set()
  * default contructor that sets the count to 0
  * and all of the children to null pointer
  * parameter: none
  * return: none
  * *******************************************/
template <typename T>
set<T>::set()
{
    data_count = 0;
    child_count = 0;
    for (int i = 0; i < MAXIMUM + 2; i++)
    {
        subset[i] = nullptr;
    }
    dups_ok = false;
}

/*************************************************
* BIG THREE FUNCTIONS
* **********************************************/

/************************************************
 * set(const set<T>& other)
 * copy constructor the copies the content of another
 * tree
 * parameter: other - tree to copy the contents of
 * return: none
 * *******************************************/
template <typename T>
set<T>::set(const set<T>& other)
{
	copy_tree(other);
}

/************************************************
 * operator =(const set<T>& RHS)
 * copies the content of another tree and applies it to
 * this tree
 * parameter: RHS - tree to copy the contents of
 * return: current tree
 * *******************************************/
template <typename T>
set<T>& set<T>::operator =(const set<T>& RHS)
{
	copy_tree(RHS);
    return *this;
}

/************************************************
 * ~set()
 * destructor that clears the content of the tree
 * parameter: none
 * return: none
 * *******************************************/
template <typename T>
set<T>::~set()
{
	clear_tree();
}

//---------------------------------------------------------------
//      P R I N T  E T C.
//---------------------------------------------------------------
/************************************************
 * print_tree(int level, std::ostream& outs) const
 * function that prints the content of the tree
 * recursively
 * parameter: int level - space that to give the current
 *                         data point
 *            ostream outs - ostream to print the data
 * return: none
 * *******************************************/
template <typename T>
void set<T>::print_tree(int level, std::ostream& outs) const {
    if (child_count > 0)
    {
        if (subset[child_count - 1] != nullptr)
        {
            subset[child_count - 1]->print_tree(level + 1, outs);
        }
    }
    //std::cout << "data_count: " << data_count << std::endl;
    //std::cout << "child_count: " << child_count << std::endl;
    for (int i = data_count - 1; i >= 0; i--)
    {
        outs << std::setw(5 * level) << data[i] << std::endl;
        if (child_count > 0)
        {
            subset[i]->print_tree(level + 1, outs);
        }
    } 
}

//------------------------------------------------
//          I N S E R T
//------------------------------------------------
/************************************************
 * insert(const T& entry)
 * function that calls a loose insert to enter a data item
 * and checks for if the root falls below the minimum 
 * data count
 * parameter: T entry - entry to enter
 * return: none
 * *******************************************/
template <typename T>
void set<T>::insert(const T& entry) {
    //std::cout << "INSERTING " << entry << std::endl;
    loose_insert(entry);
    if (data_count > MAXIMUM)
    {
        set<T>* copied_tree = new set<T>;
        copied_tree->data_count = data_count;
        for (int i = 0; i < data_count; i++)
        {
            copied_tree->data[i] = data[i];
        }
        copied_tree->child_count = child_count;
        for (int i = 0; i < child_count; i++)
        {
            copied_tree->subset[i] = subset[i];
        }

        data_count = 0;
        child_count = 1;
        subset[0] = copied_tree;
        fix_excess(0);
    }


    
    //in order for this class to be able to keep track of the number of the keys, this function (and the functions
    //  it calls ) must return a success code.
    //If we are to keep track of the number the keys (as opposed to key/values) then the success 
    //  code must distinguish between inserting a new key, or adding a new key to the existing key. 
    //  (for "dupes_ok")
    //
    //loose_insert this entry into this root.
    //loose_insert(entry) will insert entry into this tree. Once it returns, all the susets are valid
    //  set susets EXCEPT this root may have one extra data item:
    //    in this case (we have excess in the root)
    //      create a new node, copy all the contents of this root into it,
    //      clear this root node,
    //      make the new node this root's only child (subset[0])
    //
    //Then, call fix_excess on this only subset (subset[0])
}

/************************************************
 * loose_insert(const T& entry)
 * function that recursively checks for a location to 
 * enter in the data and checks if duplicates
 * are ok to enter
 * parameter: T entry - entry to enter
 * return: none
 * *******************************************/
template <typename T>
void set<T>::loose_insert(const T& entry) {
    
       int i = first_ge(data, data_count, entry);
       bool found = (i<data_count && data[i] == entry);

       if (found)
       {
           //adds multiple of one entry
           if (dups_ok)
           {
               if (data_count > 0)
               {
                   for (int k = data_count; k > i; k--)
                   {
                       swap(data[k], data[k - 1]);
                   }
               }
               data[i] = entry;
               if (data_count == 0)
               {
                   data_count = 1;
               }
               else if (data_count != 0)
               {
                   data_count++;
               }
           }
           //overwrites
           else
               data[i] = entry;
       }
       else
       {
           if (is_leaf())
           {
               if (data_count > 0)
               {
                   for (int k = data_count; k > i; k--)
                   {
                       swap(data[k], data[k - 1]);
                   }
               }
               data[i] = entry;
               if (data_count == 0)
               {
                   data_count = 1;
               }
               else if (data_count != 0)
               {
                   data_count++;
               }
           }
           else
           {
               subset[i]->loose_insert(entry);
               if (subset[i]->data_count > MAXIMUM)
               {
                   fix_excess(i);
               }
           }
       }
       /*
       three cases:
         a. found: deal with duplicates
         ! found:
         b. leaf : insert entry in data at position i
         c. !leaf: subset[i]->loose_insert(entry)
                   fix_excess(i) if there is a need
            |   found     |   !found        |
      ------|-------------|-----------------|-------
      leaf  |  a. Deal    | b: insert entry |
            |     with    |    at data[i]   |
      ------|  duplicates |-----------------|-------
            |             | d: subset[i]->  |
      !leaf |             |    loose_insert |
            |             |    fix_excess(i)|
      ------|-------------|-----------------|-------
    */
}


/************************************************
 * fix_excess(int i)
 * function that splits a tree that exceeds maximum
 * capacity into separate trees and enters the
 * middle item into the current tree
 * parameter: int i - index of child to split
 * return: none
 * *******************************************/
template <typename T>
void set<T>::fix_excess(int i)
{
    if (i < child_count)
    {
        for (int k = child_count; k > i + 1; k--)
        {
            swap(subset[k], subset[k - 1]);
        }
    }
    subset[i + 1] = new set<T>;
    split(subset[i]->data, subset[i]->data_count, subset[i+1]->data, subset[i+1]->data_count);
    if(subset[i]->child_count > 1)
        split(subset[i]->subset, subset[i]->child_count, subset[i+1] -> subset, subset[i+1] -> child_count);
    T entry;
    detach_item(subset[i]->data, subset[i]->data_count, entry);
    ordered_insert(data, data_count, entry);
    child_count++;

    //this node's child i has one too many items: 3 steps: 
    //1. add a new subset at location i+1 of this node 
    //2. split subset[i] (both the subset array and the data array) and move half into 
    // subset[i+1] (this is the subset we created in step 1.) 
    //3. detach the last data item of subset[i] and bring it and insert it into this node's data[] 
    // //Note that this last step may cause this node to have too many items. This is OK. This will be 
    // dealt with at the higher recursive level. (my parent will fix it!)  
}

/************************************************
 * clear_tree()
 * function that clears the content of the current tree
 * and calls the same function in all of it's children
 * parameter: none
 * return: none
 * *******************************************/
template <typename T>
void set<T>::clear_tree()
{
    //erases all the roots
    if (child_count > 0)
    {
        for (int i = 0; i < child_count; i++)
        {
            subset[i]->clear_tree();
            delete subset[i];
        }
    }
    
    //erases the data in the given tree
    for (int i = 0; i < data_count; i++)
    {
        data[i] = NULL;
    }
    data_count = 0;
    child_count = 0;
}

/************************************************
 * copy_tree(const set<T>& other)
 * function that copies the content of the other
 * set and calls the same function for it's children
 * parameter: set other - tree to copy
 * return: none
 * *******************************************/
template <typename T>
void set<T>::copy_tree(const set<T>& other)
{
    clear_tree();
    data_count = other.data_count;
    child_count = other.child_count;
    for (int i = 0; i < data_count; i++)
    {
        data[i] = other.data[i];
    }
    for (int i = 0; i < child_count; i++)
    {
        subset[i] = new set<T>;
        subset[i]->copy_tree(*other.subset[i]);
    }
}

/************************************************
 * remove(const T& entry)
 * function that call a loose remove and 
 * then checks if the root is empty
 * after balances and shrinks the tree if so
 * parameter: T entry - value to enter
 * return: none
 * *******************************************/
template <typename T>
void set<T>::remove(const T& entry) {
    std::cout << "REMOVING " << entry << std::endl;
    loose_remove(entry);

    //shrink the tree
    if (data_count == 0)
    {
        if (!is_leaf())
        {
            //std::cout << "SHRINKING" << std::endl;
            set<T>* shrink_ptr = subset[0];
            data_count = shrink_ptr->data_count;
            for (int i = 0; i < data_count; i++)
            {
                data[i] = shrink_ptr->data[i];
            }
            child_count = shrink_ptr->child_count;
            for (int i = 0; i < child_count; i++)
            {
                subset[i] = shrink_ptr->subset[i];
            }

            //deletes content of shrink_ptr
            for (int i = 0; i < shrink_ptr->data_count; i++)
            {
                shrink_ptr -> data[i] = NULL;
            }
            for (int i = 0; i < shrink_ptr->child_count; i++)
            {
                shrink_ptr->subset[i] = nullptr;
            }
            shrink_ptr->data_count = 0;
            shrink_ptr->child_count = 0;
        }
    }

    //Loose_remove the entry from this tree.
    //once you return from loose_remove, the root (this object) may have no data and only a single subset
    //now, the tree must shrink:
    //  point a temporary pointer (shrink_ptr) and point it to this root's only subset
    //  copy all the data and subsets of this subset into the root (through shrink_ptr)
    //  now, the root contains all the data and poiners of it's old child.
    //  now, simply delete shrink_ptr (blank out child), and the tree has shrunk by one level.
    //  Note, the root node of the tree will always be the same, it's the child node we delete

}

/************************************************
 * loose_remove(const T& entry)
 * function that recursivly searches for 
 * an item to remove and then removes it. if
 * removing the item causes the data count to
 * go under minimum, then the tree gets balanced out
 * parameter: T entry - value to enter
 * return: none
 * *******************************************/
template <typename T>
void set<T>::loose_remove(const T& entry)
{
    int i = first_ge(data, data_count, entry);
    bool found = (i < data_count&& data[i] == entry);

    if (found)
    {
        //found and is leaf
        if (is_leaf())
        {
            //removes child
            data[i] = NULL;
            //std::cout << "REMOVED " << entry << std::endl;
            //moves the rest of the data to the left
            if (i+1 < data_count)
            {
                for (int k = i+1; k < data_count; k++)
                {
                    swap(data[k], data[k - 1]);
                }
            }
            data_count = data_count - 1;
        }
        //found and is not leaf
        else
        {
            T largest_child;
            
            //removes the largest child from the list
            subset[i]->remove_biggest(largest_child);

            //sets the data point to the largest child
            data[i] = largest_child;

            //std::cout << "REMOVED " << entry << std::endl;

            //incase subset i goes under min
            if (subset[i]->data_count < MINIMUM)
                fix_shortage(i);
        }
    }
    //not found
    else
    {
        //not found and is not leaf
        if (!is_leaf())
        {
            subset[i]->loose_remove(entry);
            
            if (subset[i]->data_count < MINIMUM)
            {
                fix_shortage(i);
            }
        }
    }

    /* four cases:
         a. leaf && not found target: there is nothing to do
         b. leaf && found target: just remove the target
         c. not leaf and not found target: recursive call to loose_remove
         d. not leaf and found: replace target with largest child of subset[i]

            |   !found    |   found       |
      ------|-------------|---------------|-------
      leaf  |  a: nothing | b: delete     |
            |     to do   |    target     |
      ------|-------------|---------------|-------
      !leaf | c: loose_   | d: replace    |
            |    remove   |    w/ biggest |
      ------|-------------|---------------|-------


    */
}

/************************************************
 * fix_shortage(int i)
 * function that fixes the child with a shortage
 * and determines how to fill in the space
 * parameter: int i - child with shortage
 * return: none
 * *******************************************/
template <typename T>
void set<T>::fix_shortage(int i) {
    if (i == 0)
    {
        if (subset[i + 1]->data_count > MINIMUM)
        {
            rotate_left(i+1);
        }
        else
        {
            merge_with_next_subset(i);
        } 
    }
    //i is not 0
    else
    {
        if ((i+1 < child_count) && (i - 1 >= 0))
        {
            if (subset[i + 1]->data_count > MINIMUM)
            {
                rotate_left(i+1);
            }
            else if (subset[i - 1]->data_count > MINIMUM)
            {
                rotate_right(i-1);
            }
            else if (i+1 < child_count)
            {
                merge_with_next_subset(i);
            }
            else
            {
                T dataI = data[i-1];
                detach_item(data, data_count, data[i]);
                attach_item(subset[i - 1]->data, subset[i - 1]->data_count, dataI);
                merge(subset[i - 1]->data, subset[i - 1]->data_count, subset[i]->data, subset[i]->data_count);
                merge(subset[i - 1]->subset, subset[i - 1]->child_count, subset[i]->subset, subset[i]->child_count);
                child_count = child_count - 1;
            }
        }
        else
        {
            if (subset[i - 1]->data_count > MINIMUM)
            {
                rotate_right(i-1);
            }
            else if (i + 1 < child_count)
            {
                merge_with_next_subset(i);
            }
            else
            {
                T dataI = data[i-1];
                detach_item(data, data_count, data[i]);
                attach_item(subset[i - 1]->data, subset[i - 1]->data_count, dataI);
                merge(subset[i - 1]->data, subset[i - 1]->data_count, subset[i]->data, subset[i]->data_count);
                merge(subset[i - 1]->subset, subset[i - 1]->child_count, subset[i]->subset, subset[i]->child_count);
                child_count = child_count - 1;
            }
        }
    }


    /*
     * fix shortage in suset i:
     * if child i+1 has more than MINIMUM, rotate left
     * elif child i-1 has more than MINIMUM, rotate right
     * elif there is a right child, merge child i with next child
     * else merge child i with left child
     */

}

/************************************************
 * remove_biggest(T& entry)
 * function that removes the largest item 
 * items and return the largest item in the
 * passed entry. It fixes the shortage if removing
 * the data brings it under the data minimum
 * parameter: T entry - returns the largest 
 * return: none
 * *******************************************/
template <typename T>
void set<T>::remove_biggest(T& entry) {
    if (!is_leaf())
    {
        subset[child_count - 1]->remove_biggest(entry);
        if(subset[child_count - 1]->data_count < MINIMUM)
            fix_shortage(child_count - 1);
    }
    else
    {
        detach_item(data, data_count, entry);
    }

    // Keep looking in the last suset (recursive)
    //  until you get to a leaf.
    // Then, detach the last (biggest) data item
    //
    // after the recursive call, fix shortage.

}

/************************************************
 * merge_with_next_subset(int i)
 * function that removes the data from the current tree
 * places it in the child i tree, then merges the 
 * contents of that tree with the contents of
 * the tree to the right of it
 * parameter: int i - child to merge with the next
 * return: none
 * *******************************************/
template <typename T>
void set<T>::merge_with_next_subset(int i) {

    T dataI = data[i];
    
    detach_item(data, data_count, data[i]);
    attach_item(subset[i]->data, subset[i]->data_count, dataI);
    merge(subset[i]->data, subset[i]->data_count, subset[i + 1]->data, subset[i + 1]->data_count);
    merge(subset[i]->subset, subset[i]->child_count, subset[i + 1]->subset, subset[i + 1]->child_count);
    child_count = child_count - 1;
    //delete subset i+1
    for (int k = 0; k < subset[i+1]->data_count; k++)
    {
        subset[i + 1]->data[k] = NULL;
    }
    for (int k = 0; k < subset[i + 1]->child_count; k++)
    {
        subset[i + 1]->subset[k] = nullptr;
    }
    subset[i + 1]->data_count = 0;
    subset[i + 1]->child_count = 0;

    if (i + 1 < child_count)
    {
        for (int k = i + 1; k < child_count; k++)
        {
            swap(subset[k], subset[k + 1]);
        }
    }


    /*
     *  Merge subset[i] with subset [i+1] with data[i] in the middle
     *
     *   1. remove data[i] from this object
     *   2. append it to child[i]->data
     *   3. Move all data items from subset[i+1]->data to subset[i]->data
     *   4. Move all subset pointers from subset[i+1]->subset to subset[i]->subset
     *   5. delete subset[i+1] (store in a temp ptr)
     *   6. delete temp ptr
     */

}

/************************************************
 * rotate_left(int i)
 * function that rotates the left most item of the child i
 * and places it into the current data and places
 * data's replaced item into the right of child i-1
 * parameter: int i - child to merge with the next
 * return: none
 * *******************************************/
template <typename T>
void set<T>::rotate_left(int i) {
    if (0 < i && i <child_count && subset[i]->data_count > MINIMUM)
    {
        //save location of i's left child
        set<T>* suset = subset[i] -> subset[0];

        //sets left child as parent element
        subset[i - 1]->data[subset[i - 1]->data_count] = data[i-1];

        //sets parent element as i's element
        data[i-1] = subset[i]->data[0];

        //shifts the data of i to the left
        for (int k = 0; k < subset[i]->data_count; k++)
        {
            swap(subset[i]->data[k], subset[i]->data[k + 1]);
        }
        for (int k = 0; k < subset[i]->child_count; k++)
        {
            swap(subset[i]->subset[k], subset[i]->subset[k+1]);
        }


        //sets data count of each tree
        subset[i - 1]->data_count = MINIMUM;
        if (suset != nullptr)
        {
            if (suset->data_count != 0)
            {
                subset[i - 1]->child_count = MINIMUM+1;
            }
        }
            
        subset[i]->data_count = subset[i]->data_count - 1;
        subset[i]->child_count = subset[i]->child_count - 1;
        if (subset[i]->child_count < 0)
            subset[i]->child_count = 0;

        //sets the right subset of the left child to the i
        // left child
        subset[i - 1] -> subset[MINIMUM] = suset;
    }

    /*
     * (0 < i < child_count) and (subset[i]->data_count > MINIMUM)
     * subset[i-1] has only MINIMUM - 1 entries.
     *
     * item transfers from child[i] to child [i-1]
     *
     * FIRST item of subset[i]->data moves up to data to replace data[i-1],
     * data[i-1] moves down to the RIGHT of subset[i-1]->data
     *
     *  i = 1:
     *              [50 100]
     *  [  ]        [65 75]       ....
     *            [a]  [b]  [c]
     *
     *  65 move up to replace 50 (data[i])
     *  65's child (its child 0) moves over to be the child of 50
     *  50 moves down to the right of subset[i]->data
     *
     *              [65 100]
     *  [50]         [ 75 ]       ....
     *     [a]      [b]  [c]
     *
     *
     *
     *
     */
     // If necessary, shift first subset of subset[i] to end of subset[i-1]
}

/************************************************
 * rotate_right(int i)
 * function that rotates the right most item of the child i
 * and places it into the current data and places
 * data's replaced item into the left of child i+1
 * parameter: int i - child to merge with the next
 * return: none
 * *******************************************/
template <typename T>
void set<T>::rotate_right(int i) {
    if (i<child_count - 1 && subset[i]->data_count > MINIMUM)
    {
        //save location of i's right child
        set<T>* suset = subset[i]->subset[subset[i]->data_count];

        //shift right child's data to the right
        for (int k = MINIMUM-1; k > 0; k--)
        {
            swap(subset[i + 1]->data[k], subset[i + 1]->data[k - 1]);
        }
        for (int k = MINIMUM; k > 0; k--)
        {
            swap(subset[i + 1]->subset[k], subset[i + 1]->subset[k - 1]);
        }

        //sets right child as parent element
        subset[i + 1]->data[0] = data[i];

        //sets parent element as i's element
        data[i] = subset[i]->data[subset[i]->data_count-1];

        //sets data count of each tree
        subset[i + 1]->data_count = MINIMUM;
        if (suset != nullptr)
        {
            if (suset->data_count != 0)
            {
                subset[i + 1]->child_count = subset[i + 1]->child_count + 1;
            }
        }

        subset[i]->data_count = subset[i]->data_count - 1;
        subset[i]->child_count = subset[i]->child_count - 1;
        if (subset[i]->child_count < 0)
        {
            subset[i]->child_count = 0;
        }


        //sets the right subset of the left child to the i
        // left child
        subset[i + 1]->subset[0] = suset;
    }

    /* (i < child_count - 1) and (subset[i]->data_count > MINIMUM)
     * subset[i+ 1] has only MINIMUM - 1 entries.
     *
     * item transfers from child[i] to child [i+1]
     *
     * LAST item of subset[i]->data moves up to data to replace data[i],
     * data[i] moves down to the LEFT of subset[i+1]->data
     *
     * i = 1
     *                     [50 100]
     *      [20 30]        [65 75]          [ ]
     *  [..] [..] [..]   [a] [b] [c]        [..]
     *
     *  75 moves up to replace 100 (data[i])
     *  75's child (its last child) moves over to be the (child 0) child of 100
     *  100 moves down to subset[i]->data
     *
     *                     [50 75]
     *      [20 30]          [65]          [100]
     *  [..] [..] [..]     [a] [b]        [c] [..]
     *
     *
     *
     *
     *
     */
     // If necessary, shift last subset of subset[i] to front of subset[i+1]

}

//---------------------------------------------------------------------
//            C O N T A I N S / F I N D / G E T / E T C .
//---------------------------------------------------------------------
/************************************************
 * contains(const T& entry)
 * function that searches the tree and checks 
 * if the tree contains the entry
 * parameter: T entry - entry to check is in the tree
 * return: none
 * *******************************************/
template <typename T>
bool set<T>::contains(const T& entry) {
    int i = first_ge(data, data_count, entry);
    //match is found in current tree
    if (data[i] == entry)
        return true;

    //reached end and still hasn't found entry
    if (is_leaf())
        return false;

    //entry is larger than the last entry
    if (entry > data[data_count - 1])
        subset[child_count - 1]->contains(entry);

    //if entry has an i before a value greater or equal
    else
        subset[i]->contains(entry);

}

/************************************************
 * find(const T& entry)
 * function that searches the tree and checks
 * if the tree contains the entry 
 * parameter: T entry - entry to check is in the tree
 * return: none
 * *******************************************/
template <typename T>
T* set<T>::find(const T& entry) {
    if (contains(entry))
    {
        int i = first_ge(data, data_count, entry);
        if (data[i] == entry)
        {
            return &data[i];
        }
        if (entry > data[data_count - 1])
        {
            subset[child_count - 1]->find(entry);
        }
        else
        {
            subset[i]->find(entry);
        }
    }
    else
        std::cout << "can not find value" << std::endl;
}

/************************************************
 * get(const T& entry)
 * function that inserts the item if it can not find it
 * but retreaves the item if you it is found
 * parameter: T entry - entry to find or insert in the tree
 * return: none
 * *******************************************/
template <typename T>
T& set<T>::get(const T& entry) {
    if (!contains(entry))
        insert(entry);

    return get_existing(entry);
}

/************************************************
 * get_existing(const T& entry)
 * function that recursively searches for the
 * entry in the tree and returns it
 * parameter: T entry - entry to find in the tree
 * return: none
 * *******************************************/
template <typename T>
T& set<T>::get_existing(const T& entry) {
    int i = first_ge(data, data_count, entry);
    if (data[i] == entry)
        return data[i];
    if (entry > data[data_count - 1])
        subset[child_count - 1]->find(entry);
    else
        subset[i]->find(entry);
}

/************************************************
 * size()
 * function that returns the data count of the tree
 * parameter: none
 * return: data_count
 * *******************************************/
template <typename T>
int set<T>::size() const
{
    return data_count;
}

/************************************************
 * empty()
 * function that returns bool if the current tree
 * is empty
 * parameter: none
 * return: true if data_count = 0
 * *******************************************/
template <typename T>
bool set<T>::empty() const
{
    if (data_count == 0)
        return true;
    return false;
}

/************************************************
 * is_valid()
 * testing function that checks if the tree has any
 * error in counting the data points or
 * children
 * parameter: none
 * return: false if any conditions for breaking
 *          are meet
 * *******************************************/
template <typename T>
bool set<T>::is_valid()
{
    if (data_count == 0 && child_count != 0)
    {
        std::cout << "0 DATA WITH CHILD" << std::endl;
        return false;
    }
    if (data_count > 0 && child_count != data_count + 1)
    {
        if (child_count != 0)
        {
            std::cout << "CHILD COUNT OFF" << std::endl;
            return false;
        }
    }
    if (data_count < 0)
    {
        std::cout << "DATA REACHED BELOW 0" << std::endl;
        return false;
    }
    if (child_count < 0)
    {
        std::cout << "CHILD REACHED BELOW 0" << std::endl;
        return false;
    }
    for (int i = 0; i < child_count; i++)
    {
        if (!subset[i]->is_valid())
        {
            return false;
        }
    }
    return true;
}

/************************************************
 * Union(const set<T> rhs, set<T>& newTree)
 * function that reads the current tree and 
 * the passed tree and writes into a new tree
 * all off the values of both without any
 * repeating values
 * parameter: set<T> rhs - tree to check for
 *                         duplicates and write
 *                         into the new tree
 *            set<T>& newTree - tree that gets
 *                              written into
 *                              by both trees
 * return: none
 * *******************************************/
template <typename T>
void set<T>::Union(const set<T> rhs, set<T>& newTree)
{
    newTree = *this;

    UnionRecursive(rhs, newTree);
}

/************************************************
 * UnionRecursive(const set<T> rhs, set<T>& newTree)
 * function that recursively checks the
 *  passed tree and writes into the new tree
 * all off the values that aren't already
 * within the tree
 * parameter: set<T> rhs - tree to check for
 *                         duplicates and write
 *                         into the new tree
 *            set<T>& newTree - tree that gets
 *                              written by the
 *                              rhs tree
 * return: none
 * *******************************************/
template <typename T>
void set<T>::UnionRecursive(const set<T> rhs, set<T>& newTree)
{
    //check current data
    for (int i = 0; i < rhs.data_count; i++)
    {
        if(!newTree.contains(rhs.data[i]))
            newTree.insert(rhs.data[i]);
    }

    //check children
    for (int i = 0; i < rhs.child_count; i++)
    {
        UnionRecursive(*rhs.subset[i], newTree);
    }
}

/************************************************
 * intersection(set<T> rhs, set<T>& newTree)
 * function that checks if both the current tree
 * and the passed tree contains a value. If they
 * both have a value, then the value gets added
 * into the newTree
 * parameter: set<T> rhs - tree to check for
 *                         repeated values
 *            set<T>& newTree - tree that gets
 *                              repeated values
 *                              written into it
 * return: none
 * *******************************************/
template <typename T>
void set<T>::intersection(set<T> rhs, set<T>& newTree)
{
    //clears existing data if there is any
    if(!newTree.empty())
        newTree.clear_tree();

    for (int i = 0; i < rhs.data_count; i++)
    {
        if (contains(rhs.data[i]))
        {
            newTree.insert(rhs.data[i]);
        }
    }
    for (int i = 0; i < rhs.child_count; i++)
    {
        intersection(*rhs.subset[i], newTree);
    }
}

/**********************************************
* ARRAY FUNCTIONS
**********************************************/
/************************************************
 * maximal(const T& a, const T& b)
 * function that checks for the maximum of two data
 * parameter: T a - value to compare
 *            T b - value to compare
 * return: the larger of the two values
 * *******************************************/
template <class T>
T maximal(const T& a, const T& b)
{
    if (a >= b)
        return a;
    if (a < b)
        return b;
}

/************************************************
 * swap(T& a, T& b)
 * function the swaps two passed values
 * parameter: T a - value to swap
 *            T b - value to swap
 * return: none
 * *******************************************/
template <class T>
void swap(T& a, T& b)
{
    T temp = a;
    a = b;
    b = temp;
}

/************************************************
 * index_of_maximal(T data[], int n)
 * function that finds the index of the maximum
 * value within a passed data array
 * parameter: T data[] - array to search
 *            int n - size of the array
 * return: int max_in - index of the largest value
 * *******************************************/
template <class T>
int index_of_maximal(T data[], int n)
{
    T max, temp;
    int max_in;
    for (int i = 1; i < n; i++)
    {
        temp = maximal(data[i], data[i - 1]);
        max = maximal(max, temp);
        if (max == temp)
        {
            if (max == data[i])
            {
                max_in = i;
            }
            if (max == data[i-1])
            {
                max_in = i - 1;
            }
        }
    }
    return max_in;
}

/************************************************
 * ordered_insert(T data[], int& n, T entry)
 * function that inserts entry into where it 
 * would belong in the data array
 * parameter: T data[] - array to search
 *            int n - size of the array
 *            T entry - value to enter
 * return: none
 * *******************************************/
template <class T>
void ordered_insert(T data[], int& n, T entry)
{
    if (entry > data[n - 1])
    {
        data[n] = entry;
    }
    else
    {
        int i = first_ge(data, n, entry);
        //shifts all of the data greater than the entry
        //to the right
        for (int j = n; j > i; j--)
        {
            swap(data[j - 1], data[j]);
        }
        //enters the value into empty space;
        data[i] = entry;
    }
    n++;
}

/************************************************
 * first_ge(const T data[], int n, const T& entry)
 * function that returns the index 
 * of the value that is larger or equal to entry
 * parameter: T data[] - array to search
 *            int n - size of the array
 *            T entry - value to enter
 * return: int i - location of first value
 *                  >= entry
 * *******************************************/
template <class T>
int first_ge(const T data[], int n, const T& entry)
{
    for (int i = 0; i < n; i++)
    {
        if (data[i] >= entry)
        {
            return i;
        }
    }
    return n;
}

/************************************************
 * attach_item(T data[], int& n, const T& entry)
 * function that attaches entry at the end of 
 * the data array
 * parameter: T data[] - array to enter to
 *            int n - size of the array
 *            T entry - value to enter
 * return: none
 * *******************************************/
template <class T>
void attach_item(T data[], int& n, const T& entry)
{
    data[n] = entry;
    n++;
}

/************************************************
 * insert_item(T data[], int i, int& n, T entry)
 * function that inserts the entry into location i
 * into the data array
 * parameter: T data[] - array to enter to
 *            int i - index to enter entry
 *            int n - size of the array
 *            T entry - value to enter
 * return: none
 * *******************************************/
template <class T>
void insert_item(T data[], int i, int& n, T entry)
{
    for (int j = n; j > i; j--)
    {
        swap(data[j - 1], data[j]);
    }
    data[i] = entry;
    n++;
}

/************************************************
 * detach_item(T data[], int& n, T& entry)
 * function that removes the last item in the data
 * array and places it into entry
 * parameter: T data[] - array to remove from
 *            int n - size of the array
 *            T entry - value removed
 * return: none
 * *******************************************/
template <class T>
void detach_item(T data[], int& n, T& entry)
{
    entry = data[n - 1];
    data[n - 1] = NULL;
    n--;
}

/************************************************
 * delete_item(T data[], int i, int& n, T& entry)
 * function that deletes the item in index i
 * from the data array and places it in entry
 * parameter: T data[] - array to remove from
 *            int i - index to remove
 *            int n - size of the array
 *            T entry - value removed
 * return: none
 * *******************************************/
template <class T>
void delete_item(T data[], int i, int& n, T& entry)
{
    entry = data[i];
    for (int k = i; k < n; k++)
    {
        swap(data[k], data[k+1]);
    }
    data[n] = NULL;
    n--;
}

/************************************************
 * merge(T data1[], int& n1, T data2[], int& n2)
 * function that merges the two arrays into
 * a single array
 * parameter: T data1[] - array that is getting added to
 *            int n1 - size of the first array
 *            T data2[] - array to merge to 1
 *            int n2 - size of second array
 * return: none
 * *******************************************/
template <class T>
void merge(T data1[], int& n1, T data2[], int& n2)
{
    for (int i = n1; i < n1 + n2; i++)
    {
        data1[i] = data2[i - n1];
    }
    n1 = n1 + n2;
}

/************************************************
 * merge(T data1[], int& n1, T data2[], int& n2)
 * function that splits one array into two arrays
 * and passes the split half into the other array
 * parameter: T data1[] - array that is getting split
 *            int n1 - size of the first array
 *            T data2[] - array to save other half of 1
 *            int n2 - size of second array
 * return: none
 * *******************************************/
template <class T>
void split(T data1[], int& n1, T data2[], int& n2)
{
    n2 = n1 / 2;
    if (n1 % 2 == 0)
    {
        for (int i = n2; i <= n1; i++)
        {
            data2[i - n2] = data1[i];
        }
    }
    else
    {
        for (int i = n2 + 1; i <= n1; i++)
        {
            data2[i - (n2 + 1)] = data1[i];
        }
    }
    n1 = n1 - n2;
}

/************************************************
 * copy_array(T dest[], const T src[],
    int& dest_size, int src_size)
 * function that copies the content of one array into 
 * another array
 * parameter: T dest[] - array that is written to
 *            T src[] - array the dest is copying
 *            int dest_size- size of the dest array
 *            int src_size - size of source array
 * return: none
 * *******************************************/
template <class T>
void copy_array(T dest[], const T src[],
    int& dest_size, int src_size)
{
    dest_size = src_size;
    for (int i = 0; i < src_size; i++)
    {
        dest[i] = src[i];
    }
}

/************************************************
 * print_array(const T data[], int n, int pos)
 * function that prints the contents of the array
 * parameter: T data[] - array to print
 *            int n - size of array
 *            int pos - offset to print info
 * return: none
 * *******************************************/
template <class T>
void print_array(const T data[], int n, int pos)
{
    for (int i = 0; i < n; i++)
    {
        std::cout << std::setw(5 * pos) << data[i]<<", ";
    }
    std::cout << std::endl;
}

/************************************************
 * is_gt(const T data[], int n, const T& item)
 * function that determines if all of the items in
 * data is greater than or equal to the item
 * parameter: T data[] - array to compare
 *            int n - size of array
 *            T item - item to compare
 * return: none
 * *******************************************/
template <class T>
bool is_gt(const T data[], int n, const T& item)
{
    for (int i = 0; i < n; i++)
    {
        if (item <= data[i])
            return false;
    }
    return true;
}

/************************************************
 * is_le(const T data[], int n, const T& item)
 * function that determines if all of the items in
 * data is less than the item
 * parameter: T data[] - array to compare
 *            int n - size of array
 *            T item - item to compare
 * return: none
 * *******************************************/
template <class T>
bool is_le(const T data[], int n, const T& item)
{
    for (int i = 0; i < n; i++)
    {
        if (item > data[i])
            return false;
    }
    return true;
}

/*******************************************
*TEST FUNCTIONS
********************************************/
template <typename T>
void set<T>::construct_sample()
{
    data[0] = 50;
    data[1] = 100;
    data_count = 2;
    child_count = 3;

    subset[0] = new set<T>();
    subset[1] = new set<T>();
    subset[2] = new set<T>();
    subset[0]->data_count = 1;
    subset[0]->child_count = 0;
    subset[0]->data[0] = 25;
    subset[1]->data_count = 1;
    subset[1]->child_count = 0;
    subset[1]->data[0] = 65;
    subset[2]->data_count = 1;
    subset[2]->child_count = 0;
    subset[2]->data[0] = 150;
}

template <typename T>
void set<T>::test_sample_print()
{
    construct_sample();
    std::cout << "TESTING PRINT" << std::endl;
    std::cout << "print function" << std::endl;
    print_tree();
    std::cout << "overloaded <<" << std::endl;
    std::cout << *this;
}

template <typename T>
void set<T>::test_sample_copy_constructor()
{
    std::cout << "TESTING COPY CONSTRUCTOR" << std::endl;
    set<T> aTree;
    aTree.construct_sample();
    set<T> newTree(aTree);
    std::cout << "this tree" << std::endl;
    aTree.print_tree();
    std::cout << "copied tree" << std::endl;
    newTree.print_tree();
}

template <typename T>
void set<T>::test_sample_destructor()
{
    std::cout << "TESTING DESTRUCTOR" << std::endl;
    set<T> aTree;
    aTree.construct_sample();
    aTree.print_tree();
    std::cout << "destructor called (expected empty)" << std::endl;
    aTree.print_tree();
}

template <typename T>
void set<T>::test_sample_assignment()
{
    std::cout << "TESTING ASSIGNMENT" << std::endl;
    set<T> aTree;
    set<T> newTree;
    aTree.construct_sample();
    std::cout << "base tree" << std::endl;
    aTree.print_tree();
    std::cout << "copy tree before copying (expected empty)" << std::endl;
    newTree.print_tree();
    std::cout << "copy tree after copying (expected tree)" << std::endl;
    newTree = aTree;
    newTree.print_tree();
}

template <typename T>
void set<T>::test_sample_insert()
{
    construct_sample();
    std::cout << "TESTING INSERT" << std::endl;
    std::cout << "inserting 60" << std::endl;
    insert(60);
    print_tree();
    std::cout << "inserting 75" << std::endl;
    insert(75);
    print_tree();
    std::cout << "inserting 80" << std::endl;
    insert(80);
    print_tree();
    std::cout << "inserting 85" << std::endl;
    insert(85);
    print_tree();
    std::cout << "inserting 90" << std::endl;
    insert(90);
    print_tree();
    std::cout << "inserting 95" << std::endl;
    insert(95);
    print_tree();
    std::cout << "inserting 98" << std::endl;
    insert(98);
    print_tree();
}

template <typename T>
void set<T>::test_sample_remove()
{
    construct_sample();
    std::cout << "TESTING REMOVE" << std::endl;
    std::cout << "removing 65" << std::endl;
    std::cout << "tree before removal" << std::endl;
    print_tree();
    remove(65);
    std::cout << "tree after removal" << std::endl;
    print_tree();
    construct_sample();
    std::cout << "removing 100" << std::endl;
    print_tree();
    remove(100);
    print_tree();
    construct_sample();
    std::cout << "removing 50" << std::endl;
    print_tree();
    remove(50);
    print_tree();
    construct_sample();
    std::cout << "removing 25" << std::endl;
    print_tree();
    remove(25);
    print_tree();
    construct_sample();
    std::cout << "removing 150" << std::endl;
    print_tree();
    remove(150);
    print_tree();
}

template <typename T>
void set<T>::test_sample_copy()
{
    std::cout << "TESTING COPY FUNCTION" << std::endl;
    std::cout << "creating sample tree to copy" << std::endl;
    set<T> newTree;
    newTree.construct_sample();
    newTree.insert(60);
    newTree.insert(70);
    newTree.insert(80);
    newTree.insert(90);
    copy_tree(newTree);
    std::cout << "sample tree" << std::endl;
    newTree.print_tree();
    std::cout << "copied tree" << std::endl;
    print_tree();
}

template <typename T>
void set<T>::test_sample_clear()
{
    std::cout << "TESTING SAMPLE CLEAR" << std::endl;
    construct_sample();
    print_tree();
    std::cout << "clearing tree (next line should be empty)" << std::endl;
    clear_tree();
    print_tree();
}

template <typename T>
void set<T>::test_sample_contains()
{
    construct_sample();
    std::cout << "TESTING CONTAINS FUNCTION" << std::endl;
    std::cout << "does tree contain 65?: (expected 1)" << std::endl;
    std::cout << contains(65) << std::endl;
    std::cout << "does tree contain 100? (expected 1)" << std::endl;
    std::cout << contains(100) << std::endl;
    std::cout << "does tree contain 30? (expected 0)" << std::endl;
    std::cout << contains(30) << std::endl;
}

template <typename T>
void set<T>::test_sample_find()
{
    construct_sample();
    std::cout << "TESTING CONTAINS FUNCTION" << std::endl;
    std::cout << "find 65?: (expected 65)" << std::endl;
    std::cout << *find(65) << std::endl;
    std::cout << "find 100? (expected 100)" << std::endl;
    std::cout << *find(100) << std::endl;
    std::cout << "find 30? (expected obsurd number from null pointer)" << std::endl;
    std::cout << *find(30) << std::endl;
}

template <typename T>
void set<T>::test_sample_get()
{
    construct_sample();
    std::cout << "TESTING CONTAINS FUNCTION" << std::endl;
    std::cout << "return 65?: (expected 65)" << std::endl;
    std::cout << get(65) << std::endl;
    std::cout << "return 100? (expected 100)" << std::endl;
    std::cout << get(100) << std::endl;
    std::cout << "add and return 30? (expected 30)" << std::endl;
    std::cout << get(30) << std::endl;
}

template <typename T>
void set<T>::test_sample_maximal()
{
    construct_sample();
    std::cout << "TESTING MAXIMAL" << std::endl;
    std::cout << "expected 100" << std::endl;
    std::cout << maximal(data[0], data[1]) << std::endl;
    std::cout << "reverse order of parameters, expected 100" << std::endl;
    std::cout << maximal(data[1], data[0]) << std::endl;
}

template <typename T>
void set<T>::test_sample_swap()
{
    construct_sample();
    std::cout << "TESTING SWAP" << std::endl;
    print_tree();
    std::cout << "swapping 50 and 100" << std::endl;
    swap(data[0], data[1]);
    print_tree();
    std::cout << "expected the top two data points to replace" << std::endl;
}

template <typename T>
void set<T>::test_sample_index_of_maximal()
{
    construct_sample();
    std::cout << "TESTING INDEX OF MAXIMAL" << std::endl;
    std::cout << "expected 1" << std::endl;
    std::cout << index_of_maximal(data, data_count) << std::endl;
}

template <typename T>
void set<T>::test_sample_ordered_insert()
{
    construct_sample();
    std::cout << "TESTING ORDERED INSERT" << std::endl;
    std::cout << "entering 70" << std::endl;
    ordered_insert(data, data_count, 70);
    print_tree();
    std::cout << "reseting" << std::endl;
    construct_sample();
    std::cout << "entering 120" << std::endl;
    ordered_insert(data, data_count, 120);
    print_tree();
    std::cout << "reseting" << std::endl;
    construct_sample();
    std::cout << "entering 10" << std::endl;
    ordered_insert(data, data_count, 10);
    print_tree();
}

template <typename T>
void set<T>::test_sample_first_ge()
{
    construct_sample();
    std::cout << "TESTING FIRST GE" << std::endl;
    std::cout << "testing value smaller than all of data (expected 0)" << std::endl;
    std::cout << first_ge(data, data_count, 5) << std::endl;
    std::cout << "testing value in the middle (expected 1)" << std::endl;
    std::cout << first_ge(data, data_count, 70) << std::endl;
    std::cout << "testing value larger than all data (expected 2)" << std::endl;
    std::cout << first_ge(data, data_count, 125) << std::endl;
}

template <typename T>
void set<T>::test_sample_attach_item()
{
    construct_sample();
    std::cout << "TESTING ATTACHING ITEM AT THE END OF DATA" << std::endl;
    print_tree();
    std::cout << "attaching item" << std::endl;
    attach_item(data, data_count, 125);
    print_tree();
}

template <typename T>
void set<T>::test_sample_insert_item()
{
    construct_sample();
    std::cout << "TESTING INSERT ITEM" << std::endl;
    print_tree();
    std::cout << "inserting at beginning" << std::endl;
    insert_item(data, 0, data_count, 120);
    print_tree();
    std::cout << "resetting" << std::endl;
    construct_sample();
    std::cout << "inserting in middle" << std::endl;
    insert_item(data, 1, data_count, 120);
    print_tree();
    std::cout << "resetting" << std::endl;
    construct_sample();
    std::cout << "inserting at end" << std::endl;
    insert_item(data, 2, data_count, 120);  
    print_tree();
}

template <typename T>
void set<T>::test_sample_detach_item()
{
    construct_sample();
    std::cout << "TESTING DETACHING ITEM" << std::endl;
    print_tree();
    std::cout << "detaching item (expected 100)" << std::endl;
    T temp;
    detach_item(data, data_count, temp);
    std::cout << temp << std::endl;
    std::cout << "tree after detach" << std::endl;
    print_tree();
}

template <typename T>
void set<T>::test_sample_delete_item()
{
    construct_sample();
    std::cout << "TESTING DELETE ITEM" << std::endl;
    print_tree();
    std::cout << "deleteing 50 (expected 50)" << std::endl;
    T temp;
    delete_item(data, 0, data_count, temp);
    std::cout << temp << std::endl;
    std::cout << "tree after delete" << std::endl;
    print_tree();
    std::cout << "deleteing 100 (expected 100)" << std::endl;
    delete_item(data, 0, data_count, temp);
    std::cout << temp << std::endl;
    std::cout << "tree after delete" << std::endl;
    print_tree();
}

template <typename T>
void set<T>::test_sample_merge()
{
    construct_sample();
    std::cout << "TESTING MERGE" << std::endl;
    print_tree();
    std::cout << "merging with array containing 125" << std::endl;
    T arr[1] = { 125 };
    int arrSize = 1;
    merge(data, data_count, arr, arrSize);
    print_tree();
}

template <typename T>
void set<T>::test_sample_split()
{
    construct_sample();
    std::cout << "TESTING SPLIT" << std::endl;
    print_tree();
    std::cout << "spliting data into two arrays" << std::endl;
    T arr[MAXIMUM+1];
    int arrSize;
    split(data, data_count, arr, arrSize);
    for (int i = 0; i < data_count; i++)
        std::cout << data[i] << ", ";
    std::cout << std::endl;
    for (int i = 0; i < arrSize; i++)
        std::cout << arr[i]<< ", ";
    std::cout << std::endl;
}

template <typename T>
void set<T>::test_sample_copy_array()
{
    construct_sample();
    std::cout << "TESTING COPY ARRAY" << std::endl;
    T temp[MAXIMUM+1];
    int tempSize;
    copy_array(temp, data, tempSize, data_count);
    for (int i = 0; i < tempSize; i++)
        std::cout << temp[i] <<", ";
    std::cout << std::endl;
}

template <typename T>
void set<T>::test_sample_print_array()
{
    construct_sample();
    std::cout << "TESTING PRINT ARRAY" << std::endl;
    print_array(data, data_count, 0);
}

template <typename T>
void set<T>::test_sample_is_gt()
{
    construct_sample();
    std::cout << "TESTING IS GT" << std::endl;
    std::cout << "testing value that is greater, 120 (expected 1)"<< std::endl;
    std::cout << is_gt(data, data_count, 120) << std::endl;
    std::cout << "testing value that is not greater, 60 (expected 0)" << std::endl;
    std::cout << is_gt(data, data_count, 60) << std::endl;
}

template <typename T>
void set<T>::test_sample_is_le()
{
    construct_sample();
    std::cout << "TESTING IS LE" << std::endl;
    std::cout << "testing value that is less than, 10 (expected 1)" << std::endl;
    std::cout << is_le(data, data_count, 10) << std::endl;
    std::cout << "testing value that is equal to, 50 (expected 1)" << std::endl;
    std::cout << is_le(data, data_count, 50) << std::endl;
    std::cout << "testing value that is greater than, 70 (expected 0)" << std::endl;
    std::cout << is_le(data, data_count, 70) << std::endl;
}

template <typename T>
void set<T>::test_rotate_left()
{
    //      150
    //      125
    // 100
    //      empty
    // 50
    //      25
    data[0] = 50;
    data[1] = 100;
    data_count = 2;
    child_count = 3;

    subset[0] = new set<T>();
    subset[1] = new set<T>();
    subset[2] = new set<T>();
    subset[0]->data_count = 1;
    subset[0]->child_count = 0;
    subset[0]->data[0] = 25;
    subset[1]->data_count = 0;
    subset[1]->child_count = 0;
    subset[2]->data_count = 2;
    subset[2]->child_count = 0;
    subset[2]->data[0] = 125;
    subset[2]->data[1] = 150;

    rotate_left(2);
    print_tree();
}

template <typename T>
void set<T>::test_rotate_right()
{
    //      150
    // 100
    //      empty
    // 50
    //      30
    //      25
    data[0] = 50;
    data[1] = 100;
    data_count = 2;
    child_count = 3;

    subset[0] = new set<T>();
    subset[1] = new set<T>();
    subset[2] = new set<T>();
    subset[0]->data_count = 2;
    subset[0]->child_count = 0;
    subset[0]->data[0] = 25;
    subset[0]->data[1] = 30;
    subset[1]->data_count = 0;
    subset[1]->child_count = 0;
    subset[2]->data_count = 1;
    subset[2]->child_count = 0;
    subset[2]->data[0] = 150;

    rotate_right(0);
    print_tree();
}