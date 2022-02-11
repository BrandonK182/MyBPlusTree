#include "relationoperator.h"

/************************************
 * CONSTRUCTOR
 * ***********************************/

/**********************************************
* Token()
* default constructor that sets token and type
* to empty
* parameter: none
* return: none
********************************************/
relation_operator::relation_operator()
{
    _token = " ";
    _type = 0;
}

relation_operator::relation_operator(vector<long> list)
{
    _list = list;
}

/**********************************************
* Token(string str, int type)
* contructor that writes in the passed string
* and type to the storage
* parameter: str - sting to copy as token
*            type - int representing type of token
* return: none
********************************************/
relation_operator::relation_operator(string str, int type)
{
    _token = str;
    if(str == "")
        _token = " ";
    _type = type;
}

/**********************************************
* operator <<(ostream& outs, const Token& t)
* overloading operator to write info of the
* token
* parameter: outs - ostream
*            t - token to write
* return: ostream
********************************************/
ostream& operator <<(ostream& outs, const relation_operator& t)
{
    outs<<"|"<<t._token<<"|";
    return outs;
}
