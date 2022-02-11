#include "token.h"

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
Token::Token()
{
    _token = " ";
    _type = 0;
}

/**********************************************
* Token(string str, int type)
* contructor that writes in the passed string
* and type to the storage
* parameter: str - sting to copy as token
*            type - int representing type of token
* return: none
********************************************/
Token::Token(string str, int type)
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
ostream& operator <<(ostream& outs, const Token& t)
{
    outs<<"|"<<t._token<<"|";
    return outs;
}

/**********************************************
* type()
* function that returns the type of the token
* parameter: none
* return: type - int representing the type
********************************************/
int Token::type()
{
    return _type;
}

/**********************************************
* type_string()
* function that returns the type of the token
* as a string decribing it
* parameter: none
* return: type - string representing the type
********************************************/
string Token::type_string()
{
    string type_table[7] = {"NULL", "ALPHA", "NUMBER", "PUNC", "SPACE", "SPECIAL", "???"};
    switch (_type)
    {
        case 0:
            return type_table[0];
            break;
        case 1:
            return type_table[1];
            break;
        case 10:
            return type_table[2];
            break;
        case 12:
            return type_table[2];
            break;
        case 20:
            return type_table[3];
            break;
        case 30:
            return type_table[4];
            break;
        case 40:
            return type_table[5];
            break;
        default:
            return type_table[6];
            break;
    }
}

/**********************************************
* token_str()
* function that returns the token
* parameter: none
* return: _token - string stored in token
********************************************/
string Token::token_str()
{
    return _token;
}
