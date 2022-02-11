#pragma once
#ifndef TABLE_CPP
#define TABLE_CPP
#include "table.h"


/************************************
 * CONSTRUCTOR
 * ***********************************/

 /**********************************************
 * Table()
 * default constructor that does nothing
 * parameter: none
 * return: none
 ********************************************/
Table::Table()
{

}

/**********************************************
* Table(string fname, vector<string> field_list)
* constructor that builds a table with the given
* name and then places the provided field into
* the created table
* parameter: string fname -name of the table to create
*				vector<string> field_list - list of fields
*								to write into the table
* return: none
********************************************/
Table::Table(string fname, vector<string> field_list)
{
	//adds bin to give the file a file type
	tablename = fname;
	filename = fname + ".txt";
	last_rec = 0;
	set_fields(field_list);
	serial = 0;
	serial++;
	num_fields = field_list.size();

	for (int i = 0; i < num_fields; i++)
	{
		field_index.insert(field_list[i], i);
	}

	//open file and read in entries
	fstream f;
	Record r(field_list);
	open_fileW(f, filename.c_str());
	r.write(f);

	f.close();
}

/**********************************************
* Table(string fname)
* constructor that reads an already existing table file
* and reads in values into the table
* parameter: string fname -name of the table to read
* return: none
********************************************/
Table::Table(string fname)
{
	vector<string> field_list;
	fstream f;
	Record temp;
	vector<string> placeholder;
	for (int i = 0; i < temp.MAX_COL; i++)
	{
		placeholder.push_back("temp");
	}
	Record reader(placeholder);
	tablename = fname;
	filename = fname + ".txt";
	int record_num = 0;
	serial = 0;
	serial++;

	placeholder.clear();

	//determines field size
	open_fileRW(f, filename.c_str());
	reader.read(f, 0);
	vector<string> line = reader.get_record();
	for (int i = 0; i < line.size(); i++)
	{
		placeholder.push_back("temp");
		if (line[i][5] == 'Ì')
		{
			reader.field_num = i;
			num_fields = i;
			i = reader.MAX_COL;
		}
	}

	Record r(placeholder);
	while (!f.eof()) {
		r.read(f, record_num);
		record_num++;
	}
	last_rec = record_num - 1;
	int k = 0;
	f.close();
	open_fileRW(f, filename.c_str());
	while (k < last_rec - 1) {
		r.read(f, k);
		if (k == 0)
		{
			field_list = r.get_record();
		}
		k++;
	}

	for (int i = 0; i < num_fields; i++)
	{
		field_index.insert(field_list[i], i);
	}

	set_fields(field_list);

	f.close();
}

/**********************************************
* insert_into(vector<string> value_list)
* function that adds a vector of field entries
* into the table as a new record
* parameter: vector<string> value_list - entries to add
* return: int last_rec - last record read 
********************************************/
int Table::insert_into(vector<string> value_list)
{
	fstream f;
	open_fileRW(f, filename.c_str());
	Record r(value_list);
	r.write(f);
	f.close();
	last_rec++;

	//inserts the record number into each of the corresponding indicies
	for (int i = 0; i < num_fields; i++)
	{
		indicies[i].insert(value_list[i], last_rec);
	}

	return last_rec;
}


/**********************************************
* select_all()
* function that returns this table
* parameter: none
* return: this table
********************************************/
Table Table::select_all()
{
	return *this;
}

/**********************************************
* select(vector<string> field_list)
* function that returns a table with the
* columns of the table filtered to fit the
* passed entires
* parameter: vector<string> field_list - vector of column filters
* return: new table with filtered columns
********************************************/
Table Table::select(vector<string> field_list)
{
	fstream f;
	Record r(fields);
	string newname = tablename + to_string(serial);
	open_fileRW(f, filename.c_str());
	int record_num = 0;

	Table newTable(newname, field_list);

	//checks each request if it exists in the table
	vector<int> index;
	bool contains = false;
	bool contain_trigger = false;
	for (int i = 0; i < field_list.size(); i++)
	{
		contains = false;
		for (int k = 0; k < fields.size(); k++)
		{
			if (fields[k] == field_list[i])
			{
				contains = true;
			}
		}

		if (contains)
			index.push_back(field_index[field_list[i]]);
		else
		{
			cout << "ERROR READING IN \"" << field_list[i] << "\"" << endl;
			contain_trigger = true;
		}

	}

	if (!contain_trigger)
	{
		while (record_num <= last_rec) {
			r.read(f, record_num);
			if (record_num != 0)
			{
				newTable.insert_into(r.filter(index));
			}
			record_num++;
		}
	}

	f.close();

	return newTable;
}

/**********************************************
* select(vector<string> field_list, vector<long> record_numbers)
* function that returns a table with the
* columns of the table filtered to fit the
* passed entires and a list of record numbers to read
* that have already been filtered
* parameter: vector<string> field_list - vector of column filters
*			vector<long> record_numbers - list of filtered record numbers
* return: new table with filtered columns
********************************************/
Table Table::select(vector<string> field_list, vector<long> record_numbers)
{
	fstream f;
	Record r(fields);
	string newname = tablename + to_string(serial);
	open_fileRW(f, filename.c_str());

	//checks each request if it exists in the table
	if (field_list[0] != "*")
	{
		Table newTable(newname, field_list);
		vector<int> index;
		bool contains = false;
		bool contain_trigger = false;
		for (int i = 0; i < field_list.size(); i++)
		{
			contains = false;
			for (int k = 0; k < fields.size(); k++)
			{
				if (fields[k] == field_list[i])
				{
					contains = true;
				}
			}

			if (contains)
				index.push_back(field_index[field_list[i]]);
			else
			{
				cout << "ERROR READING IN \"" << field_list[i] << "\"" << endl;
				contain_trigger = true;
			}
		}

		if (!contain_trigger)
		{
			//change so that it selects from list
			for (int i = 0; i < record_numbers.size(); i++)
			{
				r.read(f, record_numbers[i]);
				newTable.insert_into(r.filter(index));
			}
		}

		f.close();

		return newTable;
	}
	else
	{
		Table newTable(newname, fields);
		for (int i = 0; i < record_numbers.size(); i++)
		{
			r.read(f, record_numbers[i]);
			newTable.insert_into(r.get_record());
		}
		f.close();
		return newTable;
	}
}

/**********************************************
* delete_table()
* function that deletes the file of the table
* parameter: none
* return: none
********************************************/
void Table::delete_table()
{
	fstream f;
	open_fileW(f, filename.c_str());
	f.close();
}

/**********************************************
* set_fields(vector<string> field_list)
* function that sets the field of the table
* parameter: vector<string> field_list - fields to enter
* return: none
********************************************/
void Table::set_fields(vector<string> field_list)
{
	fields = field_list;
	reindex();
}

/**********************************************
* get_fields()
* function that returns the fields of the table
* parameter: none
* return: vector<string> field_list - fields
********************************************/
vector<string> Table::get_fields()
{
	return fields;
}

/**********************************************
* reindex()
* function that clears the index and enters
* them in again for the case of changed tables
* parameter: none
* return: none
********************************************/
void Table::reindex()
{
	indicies.clear();
	for (int i = 0; i < fields.size(); i++)
	{
		MMap<string, int> temp;
		indicies.push_back(temp);
	}
	fstream f;
	open_fileRW(f, filename.c_str());
	Record r(fields);
	f.close();

	//inserts the record number into each of the corresponding indicies
	for (int k = 0; k <= last_rec; k++)
	{
		for (int i = 0; i < num_fields; i++)
		{
			indicies[i].insert(fields[i], k);
		}
	}
}

#endif
