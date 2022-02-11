#ifndef SQL_H
#define SQL_H

#include <string>
#include <iostream>
#include <stack>
#include "../table/table.h"
#include "../parser/parser.h"
#include "../RPN/RPN.h"

using namespace std;

class SQL
{
public:
    SQL();
    void run();
    void run(string command);
    
    void insert(string name, vector<string> values);
    void select(string name, vector<string> fields);
    void select(string name, vector<string> fields, vector<long> records);
    void create_table(string name, vector<string> fields);
    void drop_table(string name);

    void read_sample_file(string filename);

private:
    Map<string, Table> _tables;
    vector<string> _table_names;
    Table current_table;
    int linenum = 0;

    Record R;
};

void test_main();

#endif 
