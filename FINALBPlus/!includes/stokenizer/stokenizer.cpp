#include "stokenizer.h"
#include <cassert>

using namespace std;

int STokenizer::_table[MAX_ROWS][MAX_COLUMNS];

/*******************************************
 * CONSTRUCTORS
 * ****************************************/

/**********************************************
* STokenizer()
* default contructor that creates a tokenizer
* without a string to store
* parameter: none
* return: none
********************************************/
STokenizer::STokenizer()
{
    _pos = 0;
    make_table(_table);
}

/**********************************************
* STokenizer(char str[])
* contructor that creates a tokenizer
* that copies the character array into it's storage
* parameter: char str[] - string to copy
* return: none
********************************************/
STokenizer::STokenizer(char str[])
{
    strcpy(_buffer, str);
    _pos = 0;
    make_table(_table);
}

/************************************************
 * FUNCTIONS
 * ********************************************/

/**********************************************
* done()
* function that determines if the tokenizer is
* finished searching for tokens
* parameter: none
* return: true if at end of string
********************************************/
bool STokenizer::done()
{
    return _pos >= strlen(_buffer);
}

/**********************************************
* more()
* function that determines if the tokenizer
* needs to search for more tokens
* parameter: none
* return: true if more entries
********************************************/
bool STokenizer::more()
{
    return _pos < strlen(_buffer);
}

/**********************************************
* operator>>
* function that writes the token found from
* the tokenizer into the token
* parameter: STokenizer
*            Token
* return: STokenizer
********************************************/
STokenizer& operator >> (STokenizer& s, Token& t)
{
    int a_state = 0;
    string to_token = "";
    s.get_token(a_state, to_token);
    Token temp(to_token, a_state);
    t = temp;
    return s;
}

/**********************************************
* set_string(char str[])
* function that determines if the tokenizer is
* finished searching for tokens
* parameter: str[] - string to copy
* return: none
********************************************/
void STokenizer::set_string(char str[])
{
    strcpy(_buffer, str);
    _pos=0;
}

/**********************************************
* make_table(int _table[][MAX_COLUMNS])
* function that formats the stored table
* into a map for determining tokens
* parameter: _table[][] - table to write on
* return: none
********************************************/
void STokenizer::make_table(int _table[][MAX_COLUMNS])
{
    //DEFAULTING ALL POSITIONS TO -1
    for(int i=0; i<MAX_ROWS; i++)
    {
        for(int j=0; j<MAX_COLUMNS; j++)
        {
            _table[i][j] = -1;
        }
    }

    //CREATING SUCCESS STATES
    mark_fail(_table, 0, 30);
    mark_success(_table, 1, 1);
    mark_success(_table, 10, 10);
    mark_success(_table, 12, 12);
    mark_success(_table, 20, 20);
    mark_success(_table, 30, 30);
    mark_success(_table, 40, 40);

    //ALPHA
    mark_table(_table, 0, 1, 'a', 'z', 1);
    mark_table(_table, 0, 1, 'A', 'Z', 1);

    //NUMBERS
    mark_table(_table, 0, 0, '0', '9', 10);
    mark_table(_table, 10, 10, '0', '9', 10);
    mark_table(_table, 10, 10, '.', '.', 11);
    mark_table(_table, 11, 11, '0', '9', 12);
    mark_table(_table, 12, 12, '0', '9', 12);

    //PUNCTUATION
    mark_table(_table, 0, 0, '!', '/',20);
    mark_table(_table, 20, 20, '!', '/',20);
    mark_table(_table, 0, 0, ':', '@',20);
    mark_table(_table, 20, 20, ':', '@',20);
    mark_table(_table, 0, 0, '[', '`',20);
    mark_table(_table, 20, 20, '[', '`',20);
    mark_table(_table, 0, 0, '{', '~',20);
    mark_table(_table, 20, 20, '{', '~',20);

    //SPACE
    mark_table(_table, 0, 0, 32, 32, 30);
    mark_table(_table, 30, 30, 32, 32, 30);
    mark_table(_table, 0, 0, 9, 10, 30);
    mark_table(_table, 30, 30, 9, 10, 30);

    //SPECIAL
    mark_table(_table, 0, 0, 128, 255, 40);
    mark_table(_table, 40, 40, 128, 255, 40);
}

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
void STokenizer::mark_table(int _table[][MAX_COLUMNS], int from_row, int to_row, int from_col, int to_col, int number)
{
    assert(from_row<=to_row && from_col<=to_col && "from points must be before or on to points");
    for(int row = from_row; row<=to_row; row++)
    {
        for(int col = from_col; col<=to_col; col++)
        {
            _table[row][col] = number;
        }
    }
}

/**********************************************
* mark_success(int _table[][MAX_COLUMNS], int from_row, int to_row)
* helper function for marking the table for
* success states
* parameter: _table[][] - table to write on
*            from_row - row to start on
*            to_row - row to end writing
* return: none
********************************************/
void STokenizer::mark_success(int _table[][MAX_COLUMNS], int from_row, int to_row)
{
    assert(from_row<=to_row && "from points must be before or on to points");
    for(int row = from_row; row<=to_row; row++)
    {
        _table[row][0] = 1;
    }
}

/**********************************************
* mark_fail(int _table[][MAX_COLUMNS], int from_row, int to_row)
* helper function for marking the table for
* fail states
* parameter: _table[][] - table to write on
*            from_row - row to start on
*            to_row - row to end writing
* return: none
********************************************/
void STokenizer::mark_fail(int _table[][MAX_COLUMNS], int from_row, int to_row)
{
    assert(from_row<=to_row && "from points must be before or on to points");
    for(int row = from_row; row<=to_row; row++)
    {
        _table[row][0] = 0;
    }
}

/**********************************************
* get_token(int& start_state, string& token)
* function that creates a token and returns
* whether a token was found
* parameter: start_state - state the token starts on
*            token - string holding the token
* return: bool based on if a token was found
********************************************/
bool STokenizer::get_token(int& start_state, string& token)
{
    int ascii = _buffer[_pos];
    bool success = false;
    int succ_state = 0;
    int succ_pos = 0;
    int start_pos = _pos;

    while (_table[start_state][ascii] != -1 && more())
    {
        start_state = _table[start_state][ascii];
        _pos++;
        if (_table[start_state][0] == 1)
        {
            success = true;
            succ_state = start_state;
            succ_pos = _pos;
        }
        ascii = _buffer[_pos];
    }

    if (success == true)
    {
        start_state = succ_state;
        _pos = succ_pos;

        for (int i = start_pos; i < succ_pos; i++)
        {
            token = token + _buffer[i];
        }
    }

    return success;
}

/**********************************************
 * TEST FUNCTIONS
 * ***********************************************/

//GENERAL TEST
void test_tokens()
{
    char s[] = "it was the night of october 17th. pi was still 3.14.";
    STokenizer stk(s);
    Token t;
    stk>>t;
    while(stk.more())
    {
        cout<<setw(10)<<t.type_string()<<setw(10)<<t<<endl;

        t = Token();
        stk>>t;
    }
    cout<<setw(10)<<t.type_string()<<setw(10)<<t<<endl;
}
