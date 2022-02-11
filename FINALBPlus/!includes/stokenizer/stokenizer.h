#ifndef STOKENIZER_H
#define STOKENIZER_H

/***********************************************
* Author         :Brandon Kieng                *
* Assignment     :FileTokenizer                *
* CLASS          :CS 008          		       *
* SECTION        :MWR 1:00PM-3:05PM            *
* Due Date       :9/21/2020                    *
*************************************************/

#include <iostream>
#include "../token/token.h"
#include <iomanip>
#include <cstring>
#include <fstream>

const static int MAX_COLUMNS = 256;
const static int MAX_ROWS = 50;
const static int MAX_BUFFER = 100;

class STokenizer
{
public:
    STokenizer();
    STokenizer(char str[]);
    bool done();            //true: there are no more tokens
    bool more();            //true: there are more tokens
    //

    //---------------
    //extract one token (very similar to the way cin >> works)
    friend STokenizer& operator >> (STokenizer& s, Token& t);

    //set a new string as the input string
    void set_string(char str[]);

private:

    //create table for all the tokens we will recognize
    //                      (e.g. doubles, words, etc.)
    void make_table(int _table[][MAX_COLUMNS]);

    //extract the longest string that match
    //     one of the acceptable token types
    bool get_token(int& start_state, string& token);

    void mark_table(int _table[][MAX_COLUMNS], int from_row, int to_row, int from_col, int to_col, int number);
    void mark_success(int _table[][MAX_COLUMNS], int from_row, int to_row);
    void mark_fail(int _table[][MAX_COLUMNS], int from_row, int to_row);

    //---------------------------------

    char _buffer[MAX_BUFFER];       //input string
    int input_length;
    int _pos = 0;                   //current position in the string
    static int _table[MAX_ROWS][MAX_COLUMNS];
};

/**********************************************
* STokenizer()
* default contructor that creates a tokenizer
* without a string to store
* parameter: none
* return: none
********************************************/

/**********************************************
* STokenizer(char str[])
* contructor that creates a tokenizer
* that copies the character array into it's storage
* parameter: char str[] - string to copy
* return: none
********************************************/

/**********************************************
* done()
* function that determines if the tokenizer is
* finished searching for tokens
* parameter: none
* return: true if at end of string
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
* parameter: STokenizer
*            Token
* return: STokenizer
********************************************/

/**********************************************
* set_string(char str[])
* function that determines if the tokenizer is
* finished searching for tokens
* parameter: str[] - string to copy
* return: none
********************************************/

/**********************************************
* make_table(int _table[][MAX_COLUMNS])
* function that formats the stored table
* into a map for determining tokens
* parameter: _table[][] - table to write on
* return: none
********************************************/

/**********************************************
* mark_table(int _table[][MAX_COLUMNS], int from_row,
*            int to_row, int from_col, int to_col, int number)
* helper function for marking the table with values
* parameter: _table[][] - table to write on
*            from_row - row to start on
*            to_row - row to end writing
*            from_col - col to start on
*            to_col - col to end writing
* return: none
********************************************/

/**********************************************
* mark_success(int _table[][MAX_COLUMNS], int from_row, int to_row)
* helper function for marking the table for
* success states
* parameter: _table[][] - table to write on
*            from_row - row to start on
*            to_row - row to end writing
* return: none
********************************************/

/**********************************************
* mark_fail(int _table[][MAX_COLUMNS], int from_row, int to_row)
* helper function for marking the table for
* fail states
* parameter: _table[][] - table to write on
*            from_row - row to start on
*            to_row - row to end writing
* return: none
********************************************/

/**********************************************
* get_token(int& start_state, string& token)
* function that creates a token and returns
* whether a token was found
* parameter: start_state - state the token starts on
*            token - string holding the token
* return: bool based on if a token was found
********************************************/

/***********************
 * TEST FUNCTIONS
 * ***********************/
//void test_tokens();
//void test_tokens_extra_spaces();
//void test_tokens_symbols();

#endif // STRTOKENIZER_H
