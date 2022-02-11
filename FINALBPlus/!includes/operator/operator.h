#ifndef OPERATOR_H
#define OPERATOR_H

#include <iostream>
#include <string>
#include <vector>
#include "../token/token.h"

using namespace std;

class _operator : public Token
{
public:
    _operator();
    _operator(vector<long> list, string str, int type) :
        _list(list), Token(str, type)
    {
        _token = str;
        _type = type;
        _list = list;
    }
    friend ostream& operator <<(ostream& outs, const _operator& t);

    vector<long> get_list() { return _list; }

private:
    string _token;
    int _type;
    vector<long> _list;
};

#endif 
