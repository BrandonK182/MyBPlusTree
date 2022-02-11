#include "parser.h"
#ifndef PARSER_CPP
#define PARSER_CPP

int parser::_table[MAX_ROWS][MAX_COLUMNS];

/************************************
 * CONSTRUCTOR
 * ***********************************/

 /**********************************************
 * parser(string str)
 * constructor that builds a parser and parses
 * the passed string into a vector of tokens
 * and a list of commands
 * parameter: string str - string to parse
 * return: none
 ********************************************/
parser::parser(string str)
{
    build_table(_table);
    build_keyword_list(keyMap);
    char cstr[MAX_BUFFER];
    string temp;
    char c;
    temp = str;
    for(int i = 0; i < str.size(); i++)
    {
        c = str[i];
        temp[i] = tolower(c);
    }

    //lowercase entries
    strncpy(cstr, temp.c_str(), MAX_BUFFER);
    STokenizer stk(cstr);
    Token t;
    stk >> t;
    inputq.push_back(t);
    while (stk.more())
    {
        stk >> t;
        inputq.push_back(t);
    }

    //entries as inputted
    strncpy(cstr, str.c_str(), MAX_BUFFER);
    STokenizer stk2(cstr);
    stk2 >> t;
    input_plain.push_back(t);
    while (stk2.more())
    {
        stk2 >> t;
        input_plain.push_back(t);
    }

    //parses tree and flags if failed
    if (!get_parse_tree())
    {
        cout << "ERROR NOT COMPLETE STATEMENT" << endl;
        flag = false;
    }
    else
    {
        flag = true;
    }
}

/**********************************************
* set_string(string str)
* sets the parser to contain a string and parses
* the passed string into a vector of tokens
* and a list of commands
* parameter: string str - string to parse
* return: none
********************************************/
void parser::set_string(string str)
{
    //clears any existing data
    if (!inputq.empty())
        inputq.clear();
    if (!ptree.empty())
        ptree.clear();
    if (!token_list.empty())
        token_list.clear();

    char cstr[MAX_BUFFER];
    string temp;
    char c;
    temp = str;
    for (int i = 0; i < str.size(); i++)
    {
        c = str[i];
        temp[i] = tolower(c);
    }
    strncpy(cstr, temp.c_str(), MAX_BUFFER);
    STokenizer stk(cstr);
    strncpy(cstr, str.c_str(), MAX_BUFFER);
    STokenizer stk2(cstr);
    Token t;

    stk >> t;
    inputq.push_back(t);
    while (stk.more())
    {
        stk >> t;
        inputq.push_back(t);
    }

    stk2 >> t;
    while (stk2.more())
    {
        stk2 >> t;
        input_plain.push_back(t);
    }
    if (!get_parse_tree())
    {
        cout << "ERROR NOT COMPLETE STATEMENT" << endl;
        flag = false;
    }
    else
    {
        flag = true;
    }
}

/**********************************************
* get_parse_tree()
* function that parses the tokens into
* a list of commands
* parameter: none
* return: bool - if it reached a success state
********************************************/
inline bool parser::get_parse_tree()
{
    bool end_state = false;
    int current_state = 0;
    int column = 0;
    bool table_grammer = false;
    char c;
    bool contains_quote = false;
    bool first_encounter = false;
    int quote_place;
    string word;

    for (int i = 0; i < inputq.size(); i++)
    {
        contains_quote = false;
        if (inputq[i].type_string() == "PUNC")
        {
            for (int k = 0; k < inputq[i].token_str().size(); k++)
            {
                if (inputq[i].token_str()[k] == '"')
                {
                    contains_quote = true;
                    quote_place = k;
                    if (!first_encounter)
                        first_encounter = true;
                    else
                        first_encounter = false;
                }
            }
        }

        if (keyMap.contains(inputq[i].token_str()))
        {
            if (inputq[i].token_str().size() > 0)
                column = keyMap[inputq[i].token_str()][0];
        }
        else if (inputq[i].token_str()[0] == '"')
        {
            column = QUOTE;
        }
        else
        {
            column = SYMBOL;
        }
        if (_table[current_state][column] != -1)
            current_state = _table[current_state][column];
        switch (current_state)
        {
            case 0:
                break;
            //make table
            case 1:
            {
                ptree.insert("command", "make");
                if (inputq[i + 2].token_str() == "table" && !table_grammer)
                {
                    i = i + 2;
                }
                table_grammer = true;
                break;
            }
            //entering table name
            case 2:
                if (inputq[i].type_string() != "SPACE")
                    ptree.insert("table", input_plain[i].token_str());
                break;
            //field
            case 3:
            {
                break;
            }
            //entering field names
            case 4:
            {
                word = "";
                if (inputq[i].type_string() != "SPACE")
                    ptree.insert("fields", input_plain[i].token_str());
                break;
            }

            //case for multi word table name
            case 6:
            {
                if (inputq[i].type_string() != "PUNC")
                    word = word + input_plain[i].token_str();
                else if (contains_quote && !first_encounter)
                {
                    for (int k = 0; k < quote_place; k++)
                    {
                        word = word + input_plain[i].token_str()[k];
                    }
                    ptree.insert("table", word);
                    current_state = _table[current_state][QUOTE];
                }
                else if(input_plain[i].token_str()[0] != '"')
                {
                    word = word + input_plain[i].token_str();
                }
                if (!contains_quote && inputq[i + 1].token_str()[0] == '"')
                {
                    ptree.insert("table", word);
                    first_encounter = false;
                    i++;
                    current_state = _table[current_state][QUOTE];
                }
                break;
            }
            //multi field name
            case 7:
            {
                if (inputq[i].type_string() != "PUNC")
                    word = word + input_plain[i].token_str();
                else if (contains_quote && !first_encounter)
                {
                    for (int k = 0; k < quote_place; k++)
                    {
                        word = word + input_plain[i].token_str()[k];
                    }
                    ptree.insert("fields", word);
                    current_state = _table[current_state][QUOTE];
                    if (quote_place < input_plain[i].token_str().size())
                    {
                        if (input_plain[i].token_str()[quote_place + 1] == ',')
                        {
                            current_state = _table[current_state][COMMA];
                        }
                    }
                }
                else if (input_plain[i].token_str()[0] != '"')
                {
                    word = word + input_plain[i].token_str();
                }
                if (!contains_quote && inputq[i + 1].token_str()[0] == '"')
                {
                    ptree.insert("fields", word);
                    first_encounter = false;
                    i++;
                    current_state = _table[current_state][QUOTE];
                }

                break;
            }


            //insert
            case 11:
            {
                ptree.insert("command", "insert");
                if (inputq[i + 2].token_str() == "into")
                {
                    i = i + 2;
                }
                break;
            }
            //table name
            case 12:
            {
                if (inputq[i].type_string() != "SPACE")
                    ptree.insert("table", input_plain[i].token_str());
                break;
            }
            // check for "values"
            case 13:
            {
                break;
            }
            //inserting fields
            case 14:
            {
                word = "";
                if (inputq[i].type_string() != "SPACE")
                    ptree.insert("fields", input_plain[i].token_str());
                break;
            }
            //commas
            case 15:
                break;

            //case for multi word table name
            case 16:
            {
                if (inputq[i].type_string() != "PUNC")
                    word = word + input_plain[i].token_str();
                else if (contains_quote && !first_encounter)
                {
                    for (int k = 0; k < quote_place; k++)
                    {
                        word = word + input_plain[i].token_str()[k];
                    }
                    ptree.insert("table", word);
                    current_state = _table[current_state][QUOTE];
                }
                else if (input_plain[i].token_str()[0] != '"')
                {
                    word = word + input_plain[i].token_str();
                }
                if (!contains_quote && inputq[i + 1].token_str()[0] == '"')
                {
                    ptree.insert("table", word);
                    first_encounter = false;
                    i++;
                    current_state = _table[current_state][QUOTE];
                }
                break;
            }
            //multi field name
            case 17:
            {
                if (inputq[i].type_string() != "PUNC")
                    word = word + input_plain[i].token_str();
                else if (contains_quote && !first_encounter)
                {
                    string newword = "";
                    for (int k = 0; k < quote_place; k++)
                    {
                        newword = newword + input_plain[i].token_str()[k];
                    }
                    word = word + newword;
                    ptree.insert("fields", word);
                    current_state = _table[current_state][QUOTE];
                }
                else if (input_plain[i].token_str()[0] != '"')
                {
                    word = word + input_plain[i].token_str();
                }
                
                if (!contains_quote && inputq[i + 1].token_str()[0] == '"')
                {
                    ptree.insert("fields", word);
                    first_encounter = false;
                    i++;
                    current_state = _table[current_state][QUOTE];
                }
                break;
            }

            //select
            case 21:
                ptree.insert("command", "select");
                break;
            //star or words
            case 22:
            {
                word = "";
                if ((inputq[i].type_string() != "SPACE") || inputq[i].token_str() == "*")
                {
                    ptree.insert("fields", input_plain[i].token_str());
                }
                break;
            }
            //FROM
            case 23:
            {
                break;
            }
            //enters table name
            case 24:
            {
                word = "";
                if (inputq[i].type_string() != "SPACE")
                    ptree.insert("table", input_plain[i].token_str());
                if(i == inputq.size()-1)
                    ptree.insert("where", "no");
                break;
            }
            //where or comma
            case 25:
            {
                ptree.insert("where", "yes");
                break;
            }
            //left case
            case 26:
            {
                word = "";
                if (inputq[i].type_string() != "SPACE")
                {
                    ptree.insert("field_left", input_plain[i].token_str());
                    token_list.push_back(input_plain[i]);
                }
                break;
            }
            //operator
            case 27:
            {
                if (inputq[i].type_string() == "PUNC")
                {
                    ptree.insert("operator", inputq[i].token_str());
                    token_list.push_back(inputq[i]);
                }
                break;
            }
            //right case
            case 28:
            {
                word = "";
                if (inputq[i].type_string() != "SPACE")
                {
                    ptree.insert("value_right", input_plain[i].token_str());
                    token_list.push_back(input_plain[i]);
                }
                break;
            }
            //logic operator
            case 29:
            {
                if (inputq[i].type_string() != "SPACE")
                {
                    ptree.insert("logic", inputq[i].token_str());
                    token_list.push_back(inputq[i]);
                }
            }

            //multi field name
            case 31:
            {
                if (inputq[i].type_string() != "PUNC")
                    word = word + input_plain[i].token_str();
                else if (contains_quote && !first_encounter)
                {
                    for (int k = 0; k < quote_place; k++)
                    {
                        word = word + input_plain[i].token_str()[k];
                    }
                    ptree.insert("fields", word);
                    current_state = _table[current_state][QUOTE];
                    if (quote_place < input_plain[i].token_str().size())
                    {
                        if (input_plain[i].token_str()[quote_place + 1] == ',')
                        {
                            current_state = _table[current_state][COMMA];
                        }
                    }
                }
                else if (input_plain[i].token_str()[0] != '"')
                {
                    word = word + input_plain[i].token_str();
                }
                if (!contains_quote && inputq[i + 1].token_str()[0] == '"')
                {
                    i++;
                    first_encounter = false;
                    ptree.insert("fields", word);
                    current_state = _table[current_state][QUOTE];
                }

                break;
            }
            //case for multi word table name
            case 32:
            {
                if (inputq[i].type_string() != "PUNC")
                    word = word + input_plain[i].token_str();
                else if (contains_quote && !first_encounter)
                {
                    for (int k = 0; k < quote_place; k++)
                    {
                        word = word + input_plain[i].token_str()[k];
                    }
                    ptree.insert("table", word);
                    current_state = _table[current_state][QUOTE];
                }
                else if (input_plain[i].token_str()[0] != '"')
                {
                    word = word + input_plain[i].token_str();
                }
                if (!contains_quote && inputq[i + 1].token_str()[0] == '"')
                {
                    ptree.insert("table", word);
                    first_encounter = false;
                    i++;
                    current_state = _table[current_state][QUOTE];
                }
                    
                break;
            }
            //multi field name
            case 33:
            {
                if (inputq[i].type_string() != "PUNC")
                    word = word + input_plain[i].token_str();
                else if (contains_quote && !first_encounter)
                {
                    for (int k = 0; k < quote_place; k++)
                    {
                        word = word + input_plain[i].token_str()[k];
                    }
                    ptree.insert("field_left", word);
                    Token temp(word, 1);
                    token_list.push_back(temp);
                    current_state = _table[current_state][QUOTE];
                }
                else if (input_plain[i].token_str()[0] != '"')
                {
                    word = word + input_plain[i].token_str();
                }
                if (!contains_quote && inputq[i + 1].token_str()[0] == '"')
                {
                    ptree.insert("field_left", word);
                    Token temp(word, 1);
                    token_list.push_back(temp);
                    i++;
                    first_encounter = false;
                    current_state = _table[current_state][QUOTE];
                }
                break;
            }
            //multi value name
            case 34:
            {
                if (inputq[i].type_string() != "PUNC")
                    word = word + input_plain[i].token_str();
                else if (contains_quote && !first_encounter)
                {
                    for (int k = 0; k < quote_place; k++)
                    {
                        word = word + input_plain[i].token_str()[k];
                    }
                    ptree.insert("value_right", word);
                    Token temp(word, 1);
                    token_list.push_back(temp);
                    current_state = _table[current_state][QUOTE];
                }
                else if (input_plain[i].token_str()[0] != '"')
                {
                    word = word + input_plain[i].token_str();
                }
                if ( !contains_quote && inputq[i + 1].token_str()[0] == '"')
                {
                    ptree.insert("value_right", word);
                    Token temp(word, 1);
                    token_list.push_back(temp);
                    i++;
                    first_encounter = false;
                    current_state = _table[current_state][QUOTE];
                }
                break;
            }

            default:
            {
                cout << "unpredicted state encountered" << endl;
                current_state = 0;
                break;
            }
        }
    }

    return _table[current_state][0];
}

/**********************************************
* build_table(int _table[][MAX_COLUMNS])
* function that builds the table to parse
* tokens into their appropriate commands
* parameter: int _table[][MAX_COLUMNS] - table to write
* return: none
********************************************/
inline void parser::build_table(int _table[][MAX_COLUMNS])
{
    //DEFAULTING ALL POSITIONS TO -1
    for (int i = 0; i < MAX_ROWS; i++)
    {
        for (int j = 0; j < MAX_COLUMNS; j++)
        {
            _table[i][j] = -1;
        }
    }

    //CREATING SUCCESS STATES
    mark_fail(_table, 0, MAX_ROWS);
    mark_success(_table, 4, 4);
    mark_success(_table, 14, 14);
    mark_success(_table, 28, 28);
    mark_success(_table, 24, 24);

    //CREATE
    mark_table(_table, 0, MAKE, 1);
    mark_table(_table, 1, SYMBOL, 2);   //table name
    mark_table(_table, 2, FIELDS, 3);
    mark_table(_table, 3, SYMBOL, 4);   //field names
    mark_table(_table, 4, COMMA, 3);

    mark_table(_table, 2, QUOTE, 6);    //multiword table name
    mark_table(_table, 6, QUOTE, 2);

    mark_table(_table, 4, QUOTE, 7);    //multiword field name
    mark_table(_table, 7, QUOTE, 4);

    //INSERT 
    mark_table(_table, 0, INSERT, 11);
    mark_table(_table, 11, SYMBOL, 12); //table name
    mark_table(_table, 12, VALUES, 13);
    mark_table(_table, 13, SYMBOL, 14); //values
    mark_table(_table, 14, COMMA, 13);

    mark_table(_table, 12, QUOTE, 16);    //multiword table name
    mark_table(_table, 16, QUOTE, 12);

    mark_table(_table, 14, QUOTE, 17);    //multiword values name
    mark_table(_table, 17, QUOTE, 14);

    //SELECT
    mark_table(_table, 0, SELECT, 21); 
    mark_table(_table, 21, STAR, 22);   //STAR OR SYMBOL ACCEPTED
    mark_table(_table, 21, SYMBOL, 22); //fields
    mark_table(_table, 22, COMMA, 21);  //loops back if comma
    mark_table(_table, 22, FROM, 23);   
    mark_table(_table, 23, SYMBOL, 24); //table name
    mark_table(_table, 24, WHERE, 25); 
    mark_table(_table, 25, SYMBOL, 26); //field
    mark_table(_table, 26, OPERATOR, 27); //operator
    mark_table(_table, 27, SYMBOL, 28); //value
    mark_table(_table, 28, OPERATOR, 29); // logic operator
    mark_table(_table, 29, SYMBOL, 26); //field to restart back to needing operator

    mark_table(_table, 22, QUOTE, 31);  //multiword selecting fields name
    mark_table(_table, 31, QUOTE, 22);

    mark_table(_table, 24, QUOTE, 32);  //multiword table name
    mark_table(_table, 32, QUOTE, 24);

    mark_table(_table, 26, QUOTE, 33);  //multiword fields name   
    mark_table(_table, 33, QUOTE, 26);

    mark_table(_table, 28, QUOTE, 34);  //multiword value name
    mark_table(_table, 34, QUOTE, 28);
}

/**********************************************
* build_keyword_list(MMap<string, int> &parse)
* function that builds a map that can translate
* the tokens into numbers dictating columns in the
* tree
* parameter: MMap<string, int> &parse - map to translate tokens
* return: none
********************************************/
void parser::build_keyword_list(MMap<string, int> &parse)
{
    parse.insert("make", MAKE);
    parse.insert("create", MAKE);
    parse.insert("fields", FIELDS);
    parse.insert(",", COMMA);
    parse.insert("\"", QUOTE);
    parse.insert("insert", INSERT);
    parse.insert("values", VALUES);
    parse.insert("select", SELECT);
    parse.insert("*", STAR);
    parse.insert("from", FROM);
    parse.insert("where", WHERE);
    parse.insert("and", OPERATOR);
    parse.insert("or", OPERATOR);
    parse.insert(">=", OPERATOR);
    parse.insert("<=", OPERATOR);
    parse.insert("=", OPERATOR);
    parse.insert(">", OPERATOR);
    parse.insert("<", OPERATOR);
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
inline void parser::mark_table(int _table[][MAX_COLUMNS], int row, int col, int number)
{
    _table[row][col] = number;
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
inline void parser::mark_table(int _table[][MAX_COLUMNS], int from_row, int from_col, int to_row, int to_col, int number)
{
    assert(from_row <= to_row && from_col <= to_col && "from points must be before or on to points");
    for (int row = from_row; row <= to_row; row++)
    {
        for (int col = from_col; col <= to_col; col++)
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
inline void parser::mark_success(int _table[][MAX_COLUMNS], int from_row, int to_row)
{
    assert(from_row <= to_row && "from points must be before or on to points");
    for (int row = from_row; row <= to_row; row++)
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
inline void parser::mark_fail(int _table[][MAX_COLUMNS], int from_row, int to_row)
{
    assert(from_row <= to_row && "from points must be before or on to points");
    for (int row = from_row; row <= to_row; row++)
    {
        _table[row][0] = 0;
    }
}

#endif