#include "SQL.h"

/************************************
 * CONSTRUCTOR
 * ***********************************/

 /**********************************************
 * SQL()
 * default constructor that creates a storage file if none exists
 * parameter: none
 * return: none
 ********************************************/
SQL::SQL()
{
    fstream f;
    string temp = "store.txt";
    open_fileRW(f, temp.c_str());
    if (f.is_open())
    {
        char in[250];
        while (!f.eof())
        {
            f.getline(in, 250);
            string s(in);
            if (s != "")
            {
                _table_names.push_back(s);
                _tables.insert(s, Table(s));
            }
        }
    }
    f.close();
}

/**********************************************
* run()
* function that can takes in user commands and
* executes upon them
* parameter: none
* return: none
********************************************/
void SQL::run()
{
    string entry;
    bool loop = true;
    while (loop)
    {
        cout << "Table List: ";
        for (int i = 0; i < _table_names.size(); i++)
        {
            cout << _table_names[i] << ", ";
        }
        cout << endl;
        cout << " (run) to run a file | (end) to end program | (drop) to remove table" << endl;
        cout << "type a command directly to execute it" << endl;
        cout << ">";
        getline(cin, entry);
        if (entry == "run")
        {
            cout << "enter filename to run" << endl;
            cin >> entry;
            read_sample_file(entry);
        }
        else if (entry == "batch1")
        {
            read_sample_file("_!sample.txt");
        }
        else if (entry == "batch2")
        {
            read_sample_file("_!select-1.txt");
        }
        else if (entry == "drop")
        {
            cout << "enter table to remove" << endl;
            getline(cin, entry);
            drop_table(entry);
        }
        else if (entry == "end")
        {
            loop = false;
        }
        else
        {
            run(entry);
        }
        cout << "SQL DONE" << endl << endl;
    }
}

/**********************************************
* run(string command)
* function that processes the information
* that the user has entered
* parameter: string command - command to process
* return: none
********************************************/
void SQL::run(string command)
{
    if (command[0] == '/')
    {
        cout << command << endl;
    }
    else
    {
        cout << "[" << linenum << "] ";
        linenum++;
        cout << command << endl;
        parser parse(command);
        if (parse.get_flag())
        {
            MMap<string, string> ptree = parse.ptree;
            if (ptree["command"][0] == "make")
            {
                create_table(ptree["table"][0], ptree["fields"]);
                cout << "table " << ptree["table"][0] << " created. cols: ";
                for (int i = 0; i < ptree["fields"].size(); i++)
                {
                    cout << ptree["fields"][i] << " ";
                }
                cout << endl;
            }
            if (ptree["command"][0] == "insert")
            {
                insert(ptree["table"][0], ptree["fields"]);
                cout << "INSERTED" << endl;
            }
            if (ptree["command"][0] == "select")
            {
                if (ptree["where"][0] == "yes")
                {
                    string name = ptree["table"][0];
                    RPN rpn(_tables[name], parse.get_token_list());

                    select(ptree["table"][0], ptree["fields"], rpn.get_filtered());
                }
                else
                    select(ptree["table"][0], ptree["fields"]);
            }
        }
    }
}

/**********************************************
* insert(string name, vector<string> values)
* function that interts the passed name
* and values into a table of the same name 
* if created
* parameter: string name - name of table
*           vector<string> values - values to enter
*                               into the table
* return: none
********************************************/
void SQL::insert(string name, vector<string> values)
{
    //cout << "inserting" << endl;

    if (_tables.contains(name))
    {
        _tables[name].insert_into(values);
    }

    else
        cout << "error finding table of the name \"" << name << "\"" << endl;
}

/**********************************************
* select(string name, vector<string> fields)
* function that filters the table from columns
* passed in
* parameter: string name - name of table
*           vector<string> fields - fields to filter
* return: none
********************************************/
void SQL::select(string name, vector<string> fields)
{
    //cout << "selecting" << endl;
    if (_tables.contains(name))
    {
        if (fields[0] == "*")
            cout << _tables[name].select_all() << endl;
        else
            cout << _tables[name].select(fields) << endl;
    }
    else
        cout << "error finding table of the name \"" << name << "\"" << endl;
}

/**********************************************
* select(string name, vector<string> fields, vector<long> records)
* function that filters both the columns and the records
* of a table
* parameter: string name - name of table
*           vector<string> fields - fields to filter
*           vector<long> records - records to print
* return: none
********************************************/
void SQL::select(string name, vector<string> fields, vector<long> records)
{
    //cout << "selecting" << endl;
    if (_tables.contains(name))
    {
        cout << _tables[name].select(fields, records) << endl;
    }
    else
        cout << "error finding table of the name \"" << name << "\"" << endl;
}

/**********************************************
* create_table(string name, vector<string> fields)
* function that creates a new table based on the
* name and fields passed
* parameter: string name - name of table
*           vector<string> fields - fields
* return: none
********************************************/
void SQL::create_table(string name, vector<string> fields)
{
    bool contains = false;
    for (int i = 0; i < _table_names.size(); i++)
    {
        if (_table_names[i] == name)
        {
            cout << "SQL ALREADY CONTAINS " << name << endl;
            contains = true;
        }
    }
    if (!contains)
    {
        Table temp(name, fields);
        _table_names.push_back(name);
        _tables.insert(name, temp);

        fstream f;
        string tempstr = "store.txt";
        open_fileW(f, tempstr.c_str());
        for (int i = 0; i < _table_names.size(); i++)
        {
            f << _table_names[i] << endl;
        }
        cout << "TABLE " << name << " has been created" << endl;
    }
}

/**********************************************
* drop_table(string name)
* function that removes the table and removes it from
* the list
* parameter: string name - name of table to remove
* return: none
********************************************/
void SQL::drop_table(string name)
{
    _tables[name].delete_table();
    _tables.erase(name);
    for (int i = 0; i < _table_names.size(); i++)
    {
        if (_table_names[i] == name)
            _table_names.erase(_table_names.begin() + i);
    }
    fstream f;
    string temp = "store.txt";
    open_fileW(f, temp.c_str());
    for (int i = 0; i < _table_names.size(); i++)
    {
        f << _table_names[i] << endl;
    }
}

/**********************************************
* read_sample_file(string filename)
* function that reads a file and then 
* parameter: string name - name of table to remove
* return: none
********************************************/
void SQL::read_sample_file(string filename)
{
    ifstream f;
    char in[250];
    f.open(filename);
    if (f.fail())
    {
        cout << "FILE NOT FOUND" << endl;
    }
    else
    {
        while (!f.eof())
        {
            f.getline(in, 250);
            string s(in);
            if (s.size() > 0)
            {
                if (s[0] == '/')
                {
                    cout << s << endl;
                }
                else
                {
                    run(s);
                }
            }  
        }

        cout << endl << endl << endl;
        cout << "---------------END OF BATCH-------------------" << endl;
    }
    f.close();
}

 //*********************************************************************
void test_main()
{
    //SQL TESTS
    SQL sql;
    sql.run("make table student fields 	fname, 			lname, 		major, 				age");
    sql.run("insert into student values 	Flo, 			Yao, 		CS, 				20");
    sql.run("insert into student values 	Calvin, 		Woo, 		Physics,			22");
    sql.run("insert into student values 	\"Flo\", 			\"Jackson\", 	Math,	 			21");
    sql.run("insert into student values 	\"Anna Grace\", \"Del Rio\", CS, 22");
    sql.run("select * from student");
    sql.run("select * from student where fname = Flo or major = CS and age < 23");
}
