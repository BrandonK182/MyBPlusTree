#ifndef FTOKENIZER_H
#define FTOKENIZER_H

/***********************************************
* Author         :Brandon Kieng                *
* Assignment     :FileTokenizer                *
* CLASS          :CS 008          		       *
* SECTION        :MWR 1:00PM-3:05PM            *
* Due Date       :9/21/2020                    *
*************************************************/

#include "../stokenizer/stokenizer.h"

class FTokenizer
{
public:
    const int MAX_BLOCK = MAX_BUFFER;
    FTokenizer(char* fname);
    Token next_token();
    bool more();
    friend FTokenizer& operator >> (FTokenizer& f, Token& t);
private:
    bool get_new_block();
    std::ifstream _f;
    STokenizer _stk;
    char _block[MAX_BUFFER];
};


/**********************************************
* FTokenizer(char* fname)
* contructor that creates a tokenizer
* for files based on pased name
* parameter: fname - name of the file
* return: none
********************************************/

/**********************************************
* next_token()
* function generates the next token
* parameter: none
* return: token - a new token
********************************************/

/**********************************************
* more()
* function that determines if the tokenizer
* needs to search for more tokens
* parameter: none
* return: true if more entries
********************************************/

/**********************************************
* operator>>
* function that writes the token found from
* the tokenizer into the token
* parameter: FTokenizer
*            Token
* return: FTokenizer
********************************************/

/**********************************************
* get_new_block()
* function that gets the next block of text
* from the file
* parameter: none
* return: true if new black is created
********************************************/


void test_f_tokenize_simple();    //function that creates a tokenizer and reads solitude.txt

#endif // FTOKENIZER_H
