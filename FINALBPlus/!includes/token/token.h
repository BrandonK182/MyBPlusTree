#ifndef TOKEN_H
#define TOKEN_H

/***********************************************
* Author         :Brandon Kieng                *
* Assignment     :FileTokenizer                *
* CLASS          :CS 008          		       *
* SECTION        :MWR 1:00PM-3:05PM            *
* Due Date       :9/21/2020                    *
*************************************************/

#include <string>
#include <iostream>

using namespace std;

class Token
{
public:
    Token();
    Token(string str, int type);
    friend ostream& operator <<(ostream& outs, const Token& t);
    int type();
    string type_string();
    string token_str();
private:
    string _token;
    int _type;
};

/**********************************************
* Token()
* default constructor that sets token and type
* to empty
* parameter: none
* return: none
********************************************/

/**********************************************
* Token(string str, int type)
* contructor that writes in the passed string
* and type to the storage
* parameter: str - sting to copy as token
*            type - int representing type of token
* return: none
********************************************/

/**********************************************
* operator <<(ostream& outs, const Token& t)
* overloading operator to write info of the
* token
* parameter: outs - ostream
*            t - token to write
* return: ostream
********************************************/

/**********************************************
* type()
* function that returns the type of the token
* parameter: none
* return: type - int representing the type
********************************************/

/**********************************************
* type_string()
* function that returns the type of the token
* as a string decribing it
* parameter: none
* return: type - string representing the type
********************************************/

/**********************************************
* token_str()
* function that returns the token
* parameter: none
* return: _token - string stored in token
********************************************/

#endif // TOKEN_H
