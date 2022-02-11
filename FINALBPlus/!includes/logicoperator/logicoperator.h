#ifndef OPERATOR_H
#define OPERATOR_H

#include <iostream>
#include <string>
#include <vector>
#include "../token/token.h"

using namespace std;

class logic_operator : public Token
{
public:
    logic_operator();
    logic_operator(vector<long> list);
    logic_operator(string str, int type);
    friend ostream& operator <<(ostream& outs, const logic_operator& t);

    vector<long> get_list() { return _list; }

private:
    string _token;
    int _type;
    vector<long> _list;
};

#endif 
