
#include "!includes/BPlusTree/BPlusTree.h"
#include "!includes/BPlusTree/BPlusTree.cpp"
#include <iostream>
#include <cstdlib>
#include <random>
#include <assert.h>

/***********************************************
* Author         :Brandon Kieng                *
* Assignment     :BPlusTree                    *
* CLASS          :CS 008          		       *
* SECTION        :MWR 1:00PM-3:05PM            *
* Due Date       :12/7/2020                    *
*************************************************/


using namespace std;

void test_BPlusTree_auto(int tree_size = 5000, int how_many = 500, bool report = false);
bool test_BPlusTree_auto(int how_many, bool report = true);
int Random(int lo, int hi);

void test_BPlusTree_interactive();

int main()
{
    cout << endl << endl << endl << "=============================" << endl << endl << endl << endl;

    //------------------------------------------
    srand(time(0));
    //------------------------------------------


//    test_BPlusTree_insert_random();
//    test_BPlusTree_remove();
      test_BPlusTree_interactive();
//    test_BPlusTree_big_three();
//      test_BPlusTree_auto(10, 100, true);

    //    test_map();
 //      test_map_interactive();

    //    test_multimap();
 //       test_multimap_interactive();
    //    test_multimap_auto();

    //test_table();

    //    test_iterator();
    cout << endl << endl << endl << "==================================" << endl;
    return 0;
}

void test_BPlusTree_interactive()
{
    cout << "TESTING" << endl;
    BPlusTree<int> test;
    BPlusTree<int> copying;
    char input;
    int num, num2;
    bool loop = true;
    while (loop)
    {
        cout << "(i)nsert (c)lear c(o)py co(n)tains (r)emove (g)et e(x)it" << endl;
        cin >> input;
        cout << endl;
        switch (input)
        {
        case 'i':
            cout << "enter num to insert" << endl;
            cin >> num;
            cout << endl;
            test.insert(num);
            break;
        case 'c':
            cout << "clearing the tree" << endl;
            test.clear_tree();
            break;
        case 'o':
            cout << "creating a copy tree" << endl;
            copying = test;
            cout << "copied tree" << endl;
            cout << copying << endl;
            break;
        case 'r':
            cout << "enter num to remove" << endl;
            cin >> num;
            cout << endl;
            test.remove(num);
            break;
        case 'n':
            cout << "enter num to check for containing" << endl;
            cin >> num;
            cout << endl;
            if (test.contains(num))
                cout << "tree does contain " << num << endl;
            else
                cout << "tree does not contain " << num << endl;
            break;
        case 'g':
            cout << "enter key to get value" << endl;
            cin >> num;
            cout << endl;
            cout << "testing print of get" << endl;
            cout << test.get(num) << endl;
            break;
        case 'x':
            cout << "exiting" << endl;
            loop = false;
            break;
        }
        cout << test << endl;
        cout << "is valid: " << test.is_valid() <<endl;
    }

}

void test_BPlusTree_auto(int tree_size, int how_many, bool report) {
    bool verified = true;
    for (int i = 0; i < how_many; i++) {
        if (report) {
            cout << "*********************************************************" << endl;
            cout << " T E S T:    " << i << endl;
            cout << "*********************************************************" << endl;
        }
        if (!test_BPlusTree_auto(tree_size, report)) {
            cout << "T E S T :   [" << i << "]    F A I L E D ! ! !" << endl;
            verified = false;
            return;
        }

    }
    cout << "**************************************************************************" << endl;
    cout << "**************************************************************************" << endl;
    cout << "             E N D     T E S T: " << how_many << " tests of " << tree_size << " items: ";
    cout << (verified ? "VERIFIED" : "VERIFICATION FAILED") << endl;
    cout << "**************************************************************************" << endl;
    cout << "**************************************************************************" << endl;

}

bool test_BPlusTree_auto(int how_many, bool report) {
    const int MAX = 10000;
    assert(how_many < MAX);
    BPlusTree<int> bt;
    int a[MAX];
    int original[MAX];
    int deleted_list[MAX];

    int original_size;
    int size;
    int deleted_size = 0;

    //fill a[ ]
    for (int i = 0; i < how_many; i++) {
        a[i] = i;
    }
    //shuffle a[ ]: Put this in a function!
    for (int i = 0; i < how_many; i++) {
        int from = Random(0, how_many - 1);
        int to = Random(0, how_many - 1);
        int temp = a[to];
        a[to] = a[from];
        a[from] = temp;
    }
    //copy  a[ ] -> original[ ]:
    copy_array(original, a, how_many, how_many);
    size = how_many;
    original_size = how_many;
    for (int i = 0; i < size; i++) {
        bt.insert(a[i]);
    }
    if (report) {
        cout << "========================================================" << endl;
        cout << "  " << endl;
        cout << "========================================================" << endl;
        cout << bt << endl << endl;
    }
    for (int i = 0; i < how_many; i++) {
        int r = Random(0, how_many - i - 1);
        if (report) {
            cout << "========================================================" << endl;
            cout << bt << endl;
            cout << ". . . . . . . . . . . . . . . . . . . . . . . . . . . . " << endl;
            cout << "deleted: "; print_array(deleted_list, deleted_size);
            cout << "   from: "; print_array(original, original_size);
            cout << endl;
            cout << "  REMOVING [" << a[r] << "]" << endl;
            cout << "========================================================" << endl;
        }
        bt.remove(a[r]);


        delete_item(a, r, size, deleted_list[deleted_size++]);
        if (!bt.is_valid()) {
            cout << setw(6) << i << " I N V A L I D   T R E E" << endl;
            cout << "Original Array: "; print_array(original, original_size);
            cout << "Deleted Items : "; print_array(deleted_list, deleted_size);
            cout << endl << endl << bt << endl << endl;
            return false;
        }

    }
    if (report) cout << " V A L I D    T R E E" << endl;

    return true;
}

int Random(int lo, int hi)
{
    int r = rand() % (hi - lo + 1) + lo;

    return r;
}