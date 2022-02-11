#ifndef OPERATOR_H
#define OPERATOR_H

#include <iostream>
#include <string>
#include <vector>
#include "../token/token.h"

using namespace std;

class relation_operator : public Token
{
public:
    relation_operator();
    relation_operator(vector<long> list);
    relation_operator(string str, int type);
    friend ostream& operator <<(ostream& outs, const relation_operator& t);

    vector<long> get_list();

private:
    string _token;
    int _type;
    vector<long> _list;
};

#endif 
