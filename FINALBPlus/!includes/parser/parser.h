#ifndef PARSER_H
#define PARSER_H

#include "../mmap/mmap.h"
#include "../mmap/mmap.cpp"
#include "../stokenizer/stokenizer.h"
#include <iomanip>

using namespace std;

class parser
{
public:
	parser(string str);
	void set_string(string str);
	vector<Token> get_token_list() { return token_list; }
	bool get_flag() { return flag; }

	MMap<string, string> ptree;

private:
	const static int MAX_ROWS = 40;
	const static int MAX_COLUMNS = 40;
	const static int MAX_BUFFER = 1000;

	vector<Token> inputq;
	vector<Token> input_plain;

	enum _keys { MAKE = 1, TABLE = 2, FIELDS = 3, COMMA = 4, SYMBOL = 5, QUOTE = 6, INSERT = 11, VALUES = 12, SELECT = 21,
		         FROM = 22, STAR = 23, WHERE = 24, OPERATOR = 25, };
	
	bool flag;

	static int _table[MAX_ROWS][MAX_COLUMNS];
	MMap<string, int> keyMap;

	bool get_parse_tree();

	vector<Token> token_list;

	bool containing_quote(string s, int& num)
	{
		for (int k = 0; k < s.size(); k++)
		{
			if (s[k] == '"')
			{
				num = k;
				return true;
			}
		}
	}

	void mark_table(int _table[][MAX_COLUMNS], int row, int col, int number);
	void mark_table(int _table[][MAX_COLUMNS], int from_row,  int from_col, int to_row, int to_col, int number);
	void mark_success(int _table[][MAX_COLUMNS], int from_row, int to_row);
	void mark_fail(int _table[][MAX_COLUMNS], int from_row, int to_row);

	void build_table(int _table[][MAX_COLUMNS]);
	void build_keyword_list(MMap<string, int> &parse);
};

#endif