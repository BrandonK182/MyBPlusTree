#ifndef RPN_H
#define RPN_H

#include <vector>
#include <iostream>
#include <iomanip>
#include <stack>
#include <queue>
#include "../table/table.h"
#include "../token/token.h"
#include "../operator/operator.h"

using namespace std;

class RPN
{
public:
	RPN(Table aTable, vector<Token> conditions);
	void print_postfix() {
		vector<string> temp;
		while (!output.empty())
		{
			cout << output.front() << " ";
			temp.push_back(output.front());
			output.pop();
		}
		for (int k = 0; k < temp.size(); k++)
		{
			output.push(temp[k]);
		}
		cout << endl;
	}

	vector<long> get_filtered() { return filtered; }

private:
	 queue<string> shunt(vector<Token> conditions);
	 vector<long> calculate();

	 vector<long> filtered;

	 queue<string> output;
	 Table storedTable;

	 vector<long> less_than(string field, string value);
	 vector<long> greater_than(string field, string value);
	 vector<long> less_than_equal(string field, string value);
	 vector<long> greater_than_equal(string field, string value);
	 vector<long> equal_to(string field, string value);

	 void build_maps();
	 Map<string, int> presidence_map;
	 Map<string, int> relation_map;
	 enum _presidence { RELATION = 3, AND = 2, OR = 1, LESSTHAN = 51, LESSTHANEQUAL = 52, GREATERTHAN = 53, GREATERTHANEQUAL = 54, EQUAL = 55 };
};

#endif