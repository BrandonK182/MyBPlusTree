#include "ftokenizer.h"

/***********************************
 * CONSTRUCTOR
 * *********************************/

/**********************************************
* FTokenizer(char* fname)
* contructor that creates a tokenizer
* for files based on pased name
* parameter: fname - name of the file
* return: none
********************************************/
FTokenizer::FTokenizer(char* fname)
{
    _f.open(fname);
    get_new_block();
}

/************************************
 * FUNCTIONS
 * **********************************/

/**********************************************
* next_token()
* function generates the next token
* parameter: none
* return: token - a new token
********************************************/
Token FTokenizer::next_token()
{
    Token T;
    _stk>>T;
    return T;
}

/**********************************************
* more()
* function that determines if the tokenizer
* needs to search for more tokens
* parameter: none
* return: true if more entries
********************************************/
bool FTokenizer::more()
{
    if(!_f.is_open())
        return _f.is_open();
    return !_f.eof();
}

/**********************************************
* operator>>
* function that writes the token found from
* the tokenizer into the token
* parameter: FTokenizer
*            Token
* return: FTokenizer
********************************************/
FTokenizer& operator >> (FTokenizer& f, Token& t)
{
    if(f._stk.more())
    {
        t = f.next_token();
    }
    else
    {
        if(f.more())
        {
           f.get_new_block();
           t = f.next_token();
        }
    }
    return f;
}

/**********************************************
* get_new_block()
* function that gets the next block of text
* from the file
* parameter: none
* return: true if new black is created
********************************************/
bool FTokenizer::get_new_block()
{
    if(!more())
        return false;
    _f.read(_block,MAX_BLOCK-1);
    _block[_f.gcount()] = '\0';
    _stk.set_string(_block);
    return true;
}

/*****************************************
 * TEST FUNCTION
 * **************************************/
//function that creates a tokenizer and reads solitude.txt
/*
void test_f_tokenize_simple()
{
    Token t;
    FTokenizer ftk("solitude.txt");
    ftk>>t;
    int token_count = 0;
    while (ftk.more()){
        if (t.type_string()=="ALPHA" ){
            token_count++;

            cout<<setw(10)<<token_count
                <<setw(3)<<left<<":"<<setw(25)<<left<<t.token_str()
                <<t.type_string()<<endl;

        }
        ftk>>t;
    }
    cout<<"Tokens Found: "<<token_count<<endl;
    cout<<"=========="<<endl;
}
*/