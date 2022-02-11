#include "operator.h"

/************************************
 * CONSTRUCTOR
 * ***********************************/

/**********************************************
* _operator()
* default constructor that sets token and type
* to empty
* parameter: none
* return: none
********************************************/
_operator::_operator()
{
    _token = " ";
    _type = 0;
}

/**********************************************
* operator <<(ostream& outs, const Token& t)
* overloading operator to write info of the
* token
* parameter: outs - ostream
*            t - token to write
* return: ostream
********************************************/
ostream& operator <<(ostream& outs, const _operator& t)
{
    outs<<"|"<<t._token<<"|";
    return outs;
}
