#include "BPlusTree.h"

/***********************************************
 * CONSTRUCTOR
 * *********************************************/

 /************************************************
  * BPlusTree(bool dups)
  * default contructor that sets the count to 0
  * and all of the children to null pointer
  * parameter: dubs - determine if duplicates are allowed
  * return: none
  * *******************************************/
template <typename T>
BPlusTree<T>::BPlusTree(bool dups):dups_ok(dups), data_count(0),
child_count(0), next(NULL)
{
    data_count = 0;
    child_count = 0;
    next = nullptr;
    for (int i = 0; i < MAXIMUM + 2; i++)
    {
        subset[i] = nullptr;
    }
    dups_ok = dups;
}

/*************************************************
* BIG THREE FUNCTIONS
* **********************************************/

/************************************************
 * BPlusTree(const BPlusTree<T>& other)
 * copy constructor the copies the content of another
 * tree
 * parameter: other - tree to copy the contents of
 * return: none
 * *******************************************/
template <typename T>
BPlusTree<T>::BPlusTree(const BPlusTree<T>& other) :
    data_count(0), child_count(0)
{
    BPlusTree<T> temp;
    BPlusTree<T>* temp2 = &temp;
	copy_tree(other, &temp2);
}

/************************************************
 * operator =(const BPlusTree<T>& RHS)
 * copies the content of another tree and applies it to
 * this tree
 * parameter: RHS - tree to copy the contents of
 * return: current tree
 * *******************************************/
template <typename T>
BPlusTree<T>& BPlusTree<T>::operator =(const BPlusTree<T>& RHS)
{
    const bool debug = false;
    if (debug) std::cout << "** assignment operator fired!" << std::endl;
    if(!empty())
        clear_tree();
    BPlusTree<T> temp;
    BPlusTree<T>* temp2 = &temp;
	copy_tree(RHS, &temp2);
    return *this;
}

/************************************************
 * ~BPlusTree()
 * destructor that clears the content of the tree
 * parameter: none
 * return: none
 * *******************************************/
template <typename T>
BPlusTree<T>::~BPlusTree()
{
    const bool debug = false;
    if (debug) std::cout << "** destructor fired!" << std::endl;
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
void BPlusTree<T>::print_tree(int level, std::ostream& outs) const {
    if (child_count > 0)
    {
        if (subset[child_count - 1] != nullptr)
        {
            subset[child_count - 1]->print_tree(level + 1, outs);
        }
    }
    for (int i = data_count - 1; i >= 0; i--)
    {
        outs << std::setw(20 * level) << data[i] << std::endl;
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
void BPlusTree<T>::insert(const T& entry) {
    //std::cout << "INSERTING " << entry << std::endl;
    loose_insert(entry);
    if (data_count > MAXIMUM)
    {
        BPlusTree<T>* copied_tree = new BPlusTree<T>;
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

        //finds the left most child of the right tree
        //and inserts the value into the leaf
        BPlusTree<T>* lowest = subset[1];
        while (!lowest->is_leaf())
        {
            lowest = lowest->subset[0];
        }
        if (lowest->data[0] != data[0])
        {
            for (int k = lowest->data_count; k > 0; k--)
            {
                swapp(lowest->data[k], lowest->data[k - 1]);
            }
            lowest->data[0] = entry;
            lowest->data_count++;
        }

        //sets the subset next if
        if (subset[0]->next != subset[1] && subset[0]->is_leaf())
            subset[0]->next = subset[1];
    }
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
void BPlusTree<T>::loose_insert(const T& entry) {
    
       int i = first_ge(data, data_count, entry);
       bool found = (i<data_count && data[i] == entry);

       if (found)
       {
           if (!is_leaf())
           {
               subset[i+1]->loose_insert(entry);
               if (subset[i+1]->data_count > MAXIMUM)
               {
                   fix_excess(i+1);
               }
           }
           else
           {
               data[i] = data[i] + entry;
           }
       }
       else
       {
           if (is_leaf())
           {
               if (data_count > 0)
               {
                   for (int k = data_count; k > i; k--)
                   {
                       swapp(data[k], data[k - 1]);
                   }
               }
               data[i] = entry;
               data_count++;
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
void BPlusTree<T>::fix_excess(int i)
{
    if (i < child_count)
    {
        for (int k = child_count; k > i + 1; k--)
        {
            swapp(subset[k], subset[k - 1]);
        }
    }

    subset[i + 1] = new BPlusTree<T>;

    //splits the single child into two
    split(subset[i]->data, subset[i]->data_count, subset[i+1]->data, subset[i+1]->data_count);

    //splits the children
    if(subset[i]->child_count > 1)
        split(subset[i]->subset, subset[i]->child_count, subset[i+1] -> subset, subset[i+1] -> child_count);
    
    //removes the middle child entry and places it in data
    T entry;
    detach_item(subset[i]->data, subset[i]->data_count, entry);
    ordered_insert(data, data_count, entry);
    child_count++;

    //finds the left most child of the right tree
    //and inserts the value into the leaf
    BPlusTree<T>* lowest = subset[i + 1];
    while (!lowest->is_leaf())
    {
        lowest = lowest->subset[0];
    }
    if (lowest->data[0] != data[i])
    {
        for (int k = lowest->data_count; k > 0; k--)
        {
            swapp(lowest->data[k], lowest->data[k - 1]);
        }
        lowest->data[0] = entry;
        lowest->data_count++;
    }

    //reroutes pointers
    if (i != 0)
    {
        subset[i+1]->next = subset[i]->next;
        subset[i]->next = subset[i+1];
    }
    else
    {
        subset[0]->next = subset[1];
    }
}

/************************************************
 * clear_tree()
 * function that clears the content of the current tree
 * and calls the same function in all of it's children
 * parameter: none
 * return: none
 * *******************************************/
template <typename T>
void BPlusTree<T>::clear_tree()
{
    //erases all the roots
    if (child_count > 0)
    {
        for (int i = 0; i < child_count; i++)
        {
            subset[i]->clear_tree();
        }
    }
    
    //erases the data in the given tree
    data_count = 0;
    child_count = 0;
    next = nullptr;
}

/************************************************
 * copy_tree(const BPlusTree<T>& other)
 * function that copies the content of the other
 * BPlusTree and calls the same function for it's children
 * parameter: BPlusTree other - tree to copy
 * return: none
 * *******************************************/
template <typename T>
void BPlusTree<T>::copy_tree(const BPlusTree<T>& other, BPlusTree<T>** last_node)
{
    data_count = other.data_count;
    child_count = other.child_count;
    for (int i = 0; i < data_count; i++)
    {
        data[i] = other.data[i];
    }

    if (is_leaf())
    {
        if ((*last_node)->empty())
        {
            (*last_node) = this;
        }
        else
        {
            (*last_node)->next = this;
            (*last_node) = this;
        }
    }

    for (int i = 0; i < child_count; i++)
    {
        subset[i] = new BPlusTree<T>;
        subset[i]->copy_tree(*other.subset[i], last_node);
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
void BPlusTree<T>::remove(const T& entry) {
    //std::cout << "REMOVING " << entry << std::endl;
    loose_remove(entry);

    //shrink the tree
    if (data_count == 0)
    {
        if (!is_leaf())
        {
            //std::cout << "SHRINKING" << std::endl;
            //merges any remaining data below

            BPlusTree<T>* shrink_ptr = subset[0];
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

            for (int i = 0; i < shrink_ptr->child_count; i++)
            {
                shrink_ptr->subset[i] = nullptr;
            }
            shrink_ptr->data_count = 0;
            shrink_ptr->child_count = 0;

        }
    }
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
void BPlusTree<T>::loose_remove(const T& entry)
{
    int i = first_ge(data, data_count, entry);
    bool found = (i < data_count&& data[i] == entry);

    if (found)
    {
        //found and is leaf
        if (is_leaf())
        {
            //std::cout << "REMOVED " << entry << std::endl;
            //moves the rest of the data to the left
            if (i+1 < data_count)
            {
                for (int k = i+1; k < data_count; k++)
                {
                    swapp(data[k], data[k - 1]);
                }
            }
            data_count = data_count - 1;
        }
        //found and is not leaf
        else
        {
            //searches for the leaf version
            subset[i+1]->loose_remove(entry);

            data[i] = subset[i + 1]->get_smallest_node()->data[0];

            //fixes shortage
            if (child_count > 1)
            {
                if (subset[i + 1]->data_count < MINIMUM)
                {
                    fix_shortage(i + 1);
                }
            }

            data[i] = subset[i + 1]->get_smallest_node()->data[0];
            //std::cout << "REMOVED " << entry << std::endl;

        }
    }
    //not found
    else
    {
        //not found and is not leaf
        if (!is_leaf())
        {
            subset[i]->loose_remove(entry);
            
            //fix shortage
            if (child_count > 1)
            {
                if (subset[i]->data_count < MINIMUM)
                {
                    fix_shortage(i);
                }
            }
        }
    }
}

/************************************************
 * fix_shortage(int i)
 * function that fixes the child with a shortage
 * and determines how to fill in the space
 * parameter: int i - child with shortage
 * return: none
 * *******************************************/
template <typename T>
void BPlusTree<T>::fix_shortage(int i) {
    //if i = 0 so no errors when checking parameters
    if (i == 0)
    {
        if (subset[i + 1]->data_count > MINIMUM)
        {
            if (subset[i + 1]->is_leaf())
                transfer_left(i + 1);
            else
                rotate_left(i + 1);
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
                if (subset[i + 1]->is_leaf())
                    transfer_left(i + 1);
                else
                    rotate_left(i + 1);
            }
            else if (subset[i - 1]->data_count > MINIMUM)
            {
                if (subset[i - 1]->is_leaf())
                    transfer_right(i - 1);
                else
                    rotate_right(i - 1);
            }
            else if (i > 0)
            {
                merge_with_next_subset(i - 1);
            }
            else
            {
                merge_with_next_subset(i);
            }
        }
        //i is the max
        else
        {
            if (subset[i - 1]->data_count > MINIMUM)
            {
                if (subset[i - 1]->is_leaf())
                    transfer_right(i - 1);
                else
                    rotate_right(i - 1);
            }
            else if (i > 0)
            {
                merge_with_next_subset(i - 1);
            }
            else
            {
                merge_with_next_subset(i);
            }
        }
    }
}

template <typename T>
BPlusTree<T>* BPlusTree<T>::get_smallest_node()
{
    BPlusTree<T>* smallest = this;
    while (!smallest->is_leaf())
    {
        smallest = smallest->subset[0];
    }
    return smallest;
}

template <typename T>
void BPlusTree<T>::get_smallest(T& entry)
{
    entry = get_smallest_node()->data[0];
}

template <typename T>
void BPlusTree<T>::get_biggest(T& entry)
{
    BPlusTree<T>* largest = this;
    while (!largest->is_leaf())
    {
        largest = largest->subset[largest->child_count-1];
    }
    entry = largest;
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
void BPlusTree<T>::remove_biggest(T& entry) {
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
void BPlusTree<T>::merge_with_next_subset(int i) {

    //removes top node
    T entry;
    delete_item(data,i, data_count, entry);

    //check leaf
    if (!subset[i]->is_leaf())
        attach_item(subset[i]->data, subset[i]->data_count, entry);

    //combines both sets
    merge(subset[i]->data, subset[i]->data_count, subset[i + 1]->data, subset[i + 1]->data_count);
    merge(subset[i]->subset, subset[i]->child_count, subset[i + 1]->subset, subset[i + 1]->child_count);
    
    child_count = child_count - 1;

    //re routes pointers
    subset[i]->next = subset[i + 1]->next;

    //clear's right tree data
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
            swapp(subset[k], subset[k + 1]);
        }
    }
}

template <typename T>
void BPlusTree<T>::rotate_left(int i) {
    if (0 < i && i <child_count && subset[i]->data_count > MINIMUM)
    {
        //save location of i's left child
        BPlusTree<T>* subtree = subset[i]->subset[0];

        //sets left child as parent element
        subset[i - 1]->data[subset[i - 1]->data_count] = data[i - 1];

        //sets parent element as i's element
        data[i - 1] = subset[i]->data[0];

        //shifts the data of i to the left
        for (int k = 0; k < subset[i]->data_count; k++)
        {
            swapp(subset[i]->data[k], subset[i]->data[k + 1]);
        }
        for (int k = 0; k < subset[i]->child_count; k++)
        {
            swapp(subset[i]->subset[k], subset[i]->subset[k + 1]);
        }


        //sets data count of each tree
        subset[i - 1]->data_count = MINIMUM;
        if (subtree != nullptr)
        {
            if (subtree->data_count != 0)
            {
                subset[i - 1]->child_count = MINIMUM + 1;
            }
        }

        subset[i]->data_count = subset[i]->data_count - 1;
        subset[i]->child_count = subset[i]->child_count - 1;
        if (subset[i]->child_count < 0)
            subset[i]->child_count = 0;

        //sets the right subset of the left child to the i
        // left child
        subset[i - 1]->subset[MINIMUM] = subtree;
    }
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
void BPlusTree<T>::rotate_right(int i) {
    if (i<child_count - 1 && subset[i]->data_count > MINIMUM)
    {
        //save location of i's right child
        BPlusTree<T>* subtree = subset[i]->subset[subset[i]->data_count];

        //shift right child's data to the right
        for (int k = MINIMUM - 1; k > 0; k--)
        {
            swapp(subset[i + 1]->data[k], subset[i + 1]->data[k - 1]);
        }
        for (int k = MINIMUM; k > 0; k--)
        {
            swapp(subset[i + 1]->subset[k], subset[i + 1]->subset[k - 1]);
        }

        //sets right child as parent element
        subset[i + 1]->data[0] = data[i];

        //sets parent element as i's element
        data[i] = subset[i]->data[subset[i]->data_count - 1];

        //sets data count of each tree
        subset[i + 1]->data_count = MINIMUM;
        if (subtree != nullptr)
        {
            if (subtree->data_count != 0)
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
        subset[i + 1]->subset[0] = subtree;
    }
}

/************************************************
 * transfer_left(int i)
 * function that transfers the left most item of the child i
 * and places it into the current data and places
 * data's replaced item into the right of child i-1
 * parameter: int i - child to merge with the next
 * return: none
 * *******************************************/
template <typename T>
void BPlusTree<T>::transfer_left(int i) {
    if (0 < i && i <child_count && subset[i]->data_count > MINIMUM)
    {
        //save location of i's left child
        BPlusTree<T>* subTree = subset[i] -> subset[0];

        //sets left child as i's element
        subset[i - 1]->data[subset[i - 1]->data_count] =  subset[i]->data[0];

        //shifts the data of i to the left
        for (int k = 0; k < subset[i]->data_count; k++)
        {
            swapp(subset[i]->data[k], subset[i]->data[k + 1]);
        }
        for (int k = 0; k < subset[i]->child_count; k++)
        {
            swapp(subset[i]->subset[k], subset[i]->subset[k+1]);
        }


        //sets data count of each tree
        subset[i - 1]->data_count = MINIMUM;
        if (subTree != nullptr)
        {
            if (subTree->data_count != 0)
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
        subset[i - 1] -> subset[MINIMUM] = subTree;

        //replaces data i-1 with child i shifted left
        data[i - 1] = subset[i]->data[0];
    }
}

/************************************************
 * transfer_right(int i)
 * function that transfers the right most item of the child i
 * and places it into the current data and places
 * data's replaced item into the left of child i+1
 * parameter: int i - child to merge with the next
 * return: none
 * *******************************************/
template <typename T>
void BPlusTree<T>::transfer_right(int i) {
    if (i<child_count - 1 && subset[i]->data_count > MINIMUM)
    {
        //save location of i's right child
        BPlusTree<T>* subTree = subset[i]->subset[subset[i]->data_count];

        //shift right child's data to the right
        for (int k = MINIMUM-1; k > 0; k--)
        {
            swapp(subset[i + 1]->data[k], subset[i + 1]->data[k - 1]);
        }
        for (int k = MINIMUM; k > 0; k--)
        {
            swapp(subset[i + 1]->subset[k], subset[i + 1]->subset[k - 1]);
        }

        //sets right child as parent element
        subset[i + 1]->data[0] = subset[i]->data[subset[i]->data_count-1];

        //sets data count of each tree
        subset[i + 1]->data_count = MINIMUM;
        if (subTree != nullptr)
        {
            if (subTree->data_count != 0)
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
        subset[i + 1]->subset[0] = subTree;

        //sets the parent data i equal to first node of child i+1 
        data[i] = subset[i + 1]->data[0];
    }
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
bool BPlusTree<T>::contains(const T& entry) {
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

template <typename T>
bool BPlusTree<T>::contains(const T& entry) const
{
    int i = first_ge(data, data_count, entry);
    //match is found in current tree
    if (data[i] == entry)
        return true;

    //reached end and still hasn't found entry
    if (is_leaf())
        return false;

    //entry is larger than the last entry
    if (entry > data[data_count - 1])
        return subset[child_count - 1]->contains(entry);

    //if entry has an i before a value greater or equal
    else
        return subset[i]->contains(entry);
}

/************************************************
 * find(const T& entry)
 * function that searches the tree and checks
 * if the tree contains the entry 
 * parameter: T entry - entry to check is in the tree
 * return: none
 * *******************************************/
template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::find(const T& entry) {
    if (contains(entry))
    {
        int i = first_ge(data, data_count, entry);
        if (data[i] == entry && is_leaf())
        {
            Iterator iter(this, i);
            return iter;
        }
        if (entry > data[data_count - 1])
        {
            subset[child_count - 1]->find(entry);
        }
        else if (data[i] == entry)
        {
            return subset[i + 1]->find(entry);
        }
        else
        {
            return subset[i]->find(entry);
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
T& BPlusTree<T>::get(const T& entry) {
    if (!contains(entry))
        insert(entry);
    return get_existing(entry);
}

template <typename T>
const T& BPlusTree<T>::get(const T& entry) const
{
    if (!contains(entry))
    {
        std::cout << "CONST GET FOR ENTRY NOT IN TREE" << std::endl;
        return T();
    }
    else
    {
        int i = first_ge(data, data_count, entry);
        if (data[i] == entry && is_leaf())
        {
            return data[i];
        }
        if (entry > data[data_count - 1])
        {
            return subset[child_count - 1]->get(entry);
        }
        else if (data[i] == entry)
        {
            return subset[i + 1]->get(entry);
        }
        else
        {
            return subset[i]->get(entry);
        }
    }
}

/************************************************
 * get_existing(const T& entry)
 * function that recursively searches for the
 * entry in the tree and returns it
 * parameter: T entry - entry to find in the tree
 * return: none
 * *******************************************/
template <typename T>
T& BPlusTree<T>::get_existing(const T& entry) {
    int i = first_ge(data, data_count, entry);
    if (data[i] == entry && is_leaf())
    {
        return data[i];
    }
    if (entry > data[data_count - 1])
        return subset[child_count - 1]->get_existing(entry);
    else if (data[i] == entry)
        return subset[i+1]->get_existing(entry);
    else
        return subset[i]->get_existing(entry);
}

/************************************************
 * size()
 * function that returns the data count of the tree
 * parameter: none
 * return: data_count
 * *******************************************/
template <typename T>
int BPlusTree<T>::size() const
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
bool BPlusTree<T>::empty() const
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
bool BPlusTree<T>::is_valid()
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
 * Iterator begin()
 * function that returns an iterator at the 
 * left most leaf in the tree
 * parameter: none
 * return: Iterator at left leaf
 * *******************************************/
template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::begin()
{
    BPlusTree* temp = this;
    while (!temp->is_leaf())
    {
        temp = temp->subset[0];
    }
    Iterator it(temp);
    return it;
}

/************************************************
 * Iterator end()
 * function that returns an iterator pointing
 * at null values to indicated the end of the
 * leaf
 * parameter: none
 * return: Iterator at nullptr
 * *******************************************/
template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::end()
{
    Iterator it(nullptr);
    return it;
}

/************************************************
 * void print_list()
 * function that prints the  of the tree
 * leaves through an iterator
 * parameter: none
 * return: none
 * *******************************************/
template <typename T>
void BPlusTree<T>::print_list()
{
    for (Iterator it = begin(); it != end(); it++)
    {
        it.print_Iterator();
    }
    std::cout << std::endl;
}

/************************************************
 * Iterator lower_bound(const T& key)
 * function that returns an iterator pointing
 * the data at the given key
 * parameter: T key - key to compare where iterator is pointing
 * return: Iterator before key
 * *******************************************/
template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::lower_bound(const T& key)
{
    Iterator it = begin();
    Iterator previous = begin();
    for (it = begin(); it != end(); it++)
    {
        if (*it >= key)
            return it;
    }
    return end();
}

/************************************************
 * Iterator upper_bound(const T& key)
 * function that returns an iterator pointing
 * the data after a given key
 * parameter: T key - key to compare where iterator is pointing
 * return: Iterator before key
 * *******************************************/
template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::upper_bound(const T& key)
{
    Iterator it;
    for (it = begin(); it != end(); it++)
    {
        if (*it > key)
        {
            return it;
        }
    }
    return end();
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
inline T maximal(const T& a, const T& b)
{
    if (a >= b)
        return a;
    if (a < b)
        return b;
}

/************************************************
 * swapp(T& a, T& b)
 * function the swapps two passed values
 * parameter: T a - value to swapp
 *            T b - value to swapp
 * return: none
 * *******************************************/
template <class T>
inline void swapp(T& a, T& b)
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
inline int index_of_maximal(T data[], int n)
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
inline void ordered_insert(T data[], int& n, T entry)
{
    if (n > 0)
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
                swapp(data[j - 1], data[j]);
            }
            //enters the value into empty space;
            data[i] = entry;
        }
    }
    else
    {
        int i = first_ge(data, n, entry);
        //shifts all of the data greater than the entry
        //to the right
        for (int j = n; j > i; j--)
        {
            swapp(data[j - 1], data[j]);
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
inline int first_ge(const T data[], int n, const T& entry)
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
inline void attach_item(T data[], int& n, const T& entry)
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
inline void insert_item(T data[], int i, int& n, T entry)
{
    for (int j = n; j > i; j--)
    {
        swapp(data[j - 1], data[j]);
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
inline void detach_item(T data[], int& n, T& entry)
{
    entry = data[n - 1];
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
inline void delete_item(T data[], int i, int& n, T& entry)
{
    entry = data[i];
    for (int k = i; k < n-1; k++)
    {
        swapp(data[k], data[k+1]);
    }
    n = n-1;
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
inline void merge(T data1[], int& n1, T data2[], int& n2)
{
    for (int i = n1; i < n1 + n2; i++)
    {
        data1[i] = data2[i - n1];
    }
    n1 = n1 + n2;
    n2 = 0;
}

/************************************************
 * split(T data1[], int& n1, T data2[], int& n2)
 * function that splits one array into two arrays
 * and passes the split half into the other array
 * parameter: T data1[] - array that is getting split
 *            int n1 - size of the first array
 *            T data2[] - array to save other half of 1
 *            int n2 - size of second array
 * return: none
 * *******************************************/
template <class T>
inline void split(T data1[], int& n1, T data2[], int& n2)
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
        for (int i = n2 + 1; i < n1; i++)
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
inline void copy_array(T dest[], const T src[],
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
inline void print_array(const T data[], int n, int pos)
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
inline bool is_gt(const T data[], int n, const T& item)
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
inline bool is_le(const T data[], int n, const T& item)
{
    for (int i = 0; i < n; i++)
    {
        if (item > data[i])
            return false;
    }
    return true;
}

/********************************************
* EXTRA VECTOR FUNCTIONS
********************************************/
/************************************************
 * ostream& operator <<(ostream& outs, const vector<T>& list)
 * function overloads the printing operator for vectors and
 * prints each item of the vector with a comma
 * parameter: ostream outs - ostream
 *            vector<T>& list - vector containing data to print
 * return: none
 * *******************************************/
template <typename T>
std::ostream& operator <<(std::ostream& outs, const std::vector<T>& list)
{
    for (int i = 0; i < list.size(); i++)
    {
        outs << list[i] << ", ";
    }
    return outs;
}

/************************************************
 * vector<T>& operator +=(vector<T>& list, const T& addme)
 * function that allows user to add items to the vector
 * through the += operator
 * parameter: vector<T>& list - vector containing data to print
 *            T& addme - value to add to vectors
 * return: list - passed in list with new value
 * *******************************************/
template <typename T>
std::vector<T>& operator +=(std::vector<T>& list, const T& addme)
{
    list.push_back(addme);
    return list;
}

/************************************************
 * vector<T>& operator +=(vector<T>& list, const vector<T> addme)
 * function that allows user to add items from one vector
 * to the vector through the += operator
 * parameter: vector<T>& list - vector containing data to print
 *            const vector<T> addme - vector containing values to add to vector
 * return: list - passed in list with new values
 * *******************************************/
template <typename T>
std::vector<T>& operator +=(std::vector<T>& list, const std::vector<T> addme)
{
    for (int i = 0; i < addme.size(); i++)
    {
        list.push_back(addme[i]);
    }
    return list;
}

/*******************************************
*TEST FUNCTIONS
********************************************/
/* 
//FOR TESTING PURPOSES
//ONLY WORKS WITH T AS INT

template <typename T>
void BPlusTree<T>::construct_sample()
{
    if (!empty())
        clear_tree();
    insert(25);
    insert(50);
    insert(65);
    insert(100);
    insert(150);
}

template <typename T>
void BPlusTree<T>::test_sample_print()
{
    construct_sample();
    std::cout << "TESTING PRINT" << std::endl;
    std::cout << "print function" << std::endl;
    print_tree();
    std::cout << "overloaded <<" << std::endl;
    std::cout << *this;
}

template <typename T>
void BPlusTree<T>::test_sample_copy_constructor()
{
    std::cout << "TESTING COPY CONSTRUCTOR" << std::endl;
    BPlusTree<T> aTree;
    aTree.construct_sample();
    BPlusTree<T> newTree(aTree);
    std::cout << "this tree" << std::endl;
    aTree.print_tree();
    std::cout << "copied tree" << std::endl;
    newTree.print_tree();
}

template <typename T>
void BPlusTree<T>::test_sample_destructor()
{
    std::cout << "TESTING DESTRUCTOR" << std::endl;
    BPlusTree<T> aTree;
    aTree.construct_sample();
    aTree.print_tree();
    std::cout << "destructor called (expected empty)" << std::endl;
    aTree.print_tree();
}

template <typename T>
void BPlusTree<T>::test_sample_assignment()
{
    std::cout << "TESTING ASSIGNMENT" << std::endl;
    BPlusTree<T> aTree;
    BPlusTree<T> newTree;
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
void BPlusTree<T>::test_sample_insert()
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
void BPlusTree<T>::test_sample_remove()
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
    std::cout << "tree after removal" << std::endl;
    print_tree();
    construct_sample();
    std::cout << "removing 50" << std::endl;
    print_tree();
    remove(50);
    std::cout << "tree after removal" << std::endl;
    print_tree();
    construct_sample();
    std::cout << "removing 25" << std::endl;
    print_tree();
    remove(25);
    std::cout << "tree after removal" << std::endl;
    print_tree();
    construct_sample();
    std::cout << "removing 150" << std::endl;
    print_tree();
    remove(150);
    std::cout << "tree after removal" << std::endl;
    print_tree();
}

template <typename T>
void BPlusTree<T>::test_sample_copy()
{
    std::cout << "TESTING COPY FUNCTION" << std::endl;
    std::cout << "creating sample tree to copy" << std::endl;
    BPlusTree<T> newTree;
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
void BPlusTree<T>::test_sample_clear()
{
    std::cout << "TESTING SAMPLE CLEAR" << std::endl;
    construct_sample();
    print_tree();
    std::cout << "clearing tree (next line should be empty)" << std::endl;
    clear_tree();
    print_tree();
}

template <typename T>
void BPlusTree<T>::test_sample_contains()
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
void BPlusTree<T>::test_sample_find()
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
void BPlusTree<T>::test_sample_get()
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
void BPlusTree<T>::test_sample_const_get()
{
    const BPlusTree<T> test;
    test.construct_sample();
    std::cout << "TESTING CONTAINS FUNCTION" << std::endl;
    std::cout << "return 65?: (expected 65)" << std::endl;
    std::cout << test.get(65) << std::endl;
    std::cout << "return 100? (expected 100)" << std::endl;
    std::cout << test.get(100) << std::endl;
    std::cout << "add and return 30? (expected 30)" << std::endl;
    std::cout << test.get(30) << std::endl;
}

template <typename T>
void BPlusTree<T>::test_sample_maximal()
{
    construct_sample();
    std::cout << "TESTING MAXIMAL" << std::endl;
    std::cout << "expected 100" << std::endl;
    std::cout << maximal(data[0], data[1]) << std::endl;
    std::cout << "reverse order of parameters, expected 100" << std::endl;
    std::cout << maximal(data[1], data[0]) << std::endl;
}

template <typename T>
void BPlusTree<T>::test_sample_swapp()
{
    construct_sample();
    std::cout << "TESTING swapp" << std::endl;
    print_tree();
    std::cout << "swappping 50 and 100" << std::endl;
    swapp(data[0], data[1]);
    print_tree();
    std::cout << "expected the top two data points to replace" << std::endl;
}

template <typename T>
void BPlusTree<T>::test_sample_index_of_maximal()
{
    construct_sample();
    std::cout << "TESTING INDEX OF MAXIMAL" << std::endl;
    std::cout << "expected 1" << std::endl;
    std::cout << index_of_maximal(data, data_count) << std::endl;
}

template <typename T>
void BPlusTree<T>::test_sample_ordered_insert()
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
void BPlusTree<T>::test_sample_first_ge()
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
void BPlusTree<T>::test_sample_attach_item()
{
    construct_sample();
    std::cout << "TESTING ATTACHING ITEM AT THE END OF DATA" << std::endl;
    print_tree();
    std::cout << "attaching item" << std::endl;
    attach_item(data, data_count, 125);
    print_tree();
}

template <typename T>
void BPlusTree<T>::test_sample_insert_item()
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
void BPlusTree<T>::test_sample_detach_item()
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
void BPlusTree<T>::test_sample_delete_item()
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
void BPlusTree<T>::test_sample_merge()
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
void BPlusTree<T>::test_sample_split()
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
void BPlusTree<T>::test_sample_copy_array()
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
void BPlusTree<T>::test_sample_print_array()
{
    construct_sample();
    std::cout << "TESTING PRINT ARRAY" << std::endl;
    print_array(data, data_count, 0);
}

template <typename T>
void BPlusTree<T>::test_sample_is_gt()
{
    construct_sample();
    std::cout << "TESTING IS GT" << std::endl;
    std::cout << "testing value that is greater, 120 (expected 1)"<< std::endl;
    std::cout << is_gt(data, data_count, 120) << std::endl;
    std::cout << "testing value that is not greater, 60 (expected 0)" << std::endl;
    std::cout << is_gt(data, data_count, 60) << std::endl;
}

template <typename T>
void BPlusTree<T>::test_sample_is_le()
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
void BPlusTree<T>::test_transfer_left()
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

    subset[0] = new BPlusTree<T>();
    subset[1] = new BPlusTree<T>();
    subset[2] = new BPlusTree<T>();
    subset[0]->data_count = 1;
    subset[0]->child_count = 0;
    subset[0]->data[0] = 25;
    subset[1]->data_count = 0;
    subset[1]->child_count = 0;
    subset[2]->data_count = 2;
    subset[2]->child_count = 0;
    subset[2]->data[0] = 125;
    subset[2]->data[1] = 150;

    transfer_left(2);
    print_tree();
}

template <typename T>
void BPlusTree<T>::test_transfer_right()
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

    subset[0] = new BPlusTree<T>();
    subset[1] = new BPlusTree<T>();
    subset[2] = new BPlusTree<T>();
    subset[0]->data_count = 2;
    subset[0]->child_count = 0;
    subset[0]->data[0] = 25;
    subset[0]->data[1] = 30;
    subset[1]->data_count = 0;
    subset[1]->child_count = 0;
    subset[2]->data_count = 1;
    subset[2]->child_count = 0;
    subset[2]->data[0] = 150;

    transfer_right(0);
    print_tree();
}
*/