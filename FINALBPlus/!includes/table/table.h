#pragma once
#ifndef TABLE_H
#define TABLE_H

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include "../record/record.h"
#include "../map/map.h"
#include "../map/map.cpp"
#include "../mmap/mmap.h"
#include "../mmap/mmap.cpp"

using namespace std;

class Table
{
public:
    Table();
	Table(string fname, vector<string> field_list);
    Table(const string fname);

    int insert_into(vector<string> value_list);
    Table select_all();
    Table select(vector<string> field_list);
    Table select(vector<string> field_list, vector<long> record_numbers);

    void delete_table();
    void set_fields(vector<string> field_list);
    vector<string> get_fields();
    Map<string, int> get_field_index() { return field_index; }
    vector<MMap<string, int>> get_indicies() { return indicies; }

    void reindex();

    friend ostream& operator<<(ostream& outs, const Table& print_me) {
        fstream f;
        open_fileRW(f, print_me.filename.c_str());
        Record r(print_me.fields);
        outs << "TABLE: " << print_me.tablename << right << setw(30) 
            << "TOTAL RECORDS: " << print_me.last_rec << endl;
        outs << "***************************************************************************" << endl;
        r.read(f, 0);
        outs << left << setw(10) << "RECORD NO" << r << endl;
        for (int i = 1; i <= print_me.last_rec; i++) {
            r.read(f, i);
            outs << left << setw(10) << i-1 << r << endl;
        }
        outs << endl << endl;
        return outs;
    }

    //needed to use maps of table
    friend Table operator + (const Table& lhs, const Table& rhs)
    {
        return lhs;
    }

private:
    string tablename;
    string filename;
    int last_rec;
    int num_fields;
    vector<vector<string>> _Table;
    vector<string> fields;
    vector<MMap<string, int>> indicies;
    Map<string, int> field_index;
    bool empty;
    int serial;
};

#endif
