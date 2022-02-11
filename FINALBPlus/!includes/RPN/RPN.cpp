#include "RPN.h"

/************************************
 * CONSTRUCTOR
 * ***********************************/

 /**********************************************
 * RPN()
 * default constructor that builds the map for RPN,
 * shunts the passed in conditions, stores a table,
 * and then calculates the records that fits all the
 * conditions
 * parameter: Table aTable - Table to read records
              vector<Token> conditions - conditions to filter records
 * return: none
 ********************************************/
RPN::RPN(Table aTable, vector<Token> conditions)
{
    build_maps();
    output = shunt(conditions);
    storedTable = aTable;
    filtered = calculate();
}

/**********************************************
* shunt(vector<Token> conditions)
* function that shunts the conditions into RPN
* so that it can execute functions in the correct order
* parameter: vector<Token> conditions - conditions to filter records
* return: queue<string> output - RPN 
********************************************/
queue<string> RPN::shunt(vector<Token> conditions)
{
    queue<string> rpn;
    stack<string> strstack;
    int current_presidence;

    for (int i = 0; i < conditions.size(); i++)
    {
        if (conditions[i].type_string() == "PUNC" ||
            conditions[i].token_str() == "and" ||
            conditions[i].token_str() == "or")
        {
            current_presidence = presidence_map[conditions[i].token_str()];

            while ((!strstack.empty()) &&
                (presidence_map[strstack.top()] >= current_presidence) &&
                strstack.top() != "(")
            {
                rpn.push(strstack.top());
                strstack.pop();
            }

            strstack.push(conditions[i].token_str());
        }
        else if (conditions[i].token_str() == "(")
        {
            strstack.push("(");
        }
        else if (conditions[i].token_str() == ")")
        {
            while (strstack.top() != "(")
            {
                rpn.push(strstack.top());
                strstack.pop();
            }
            if (strstack.top() != "(")
            {
                strstack.pop();
            }
        }
        else {
            rpn.push(conditions[i].token_str());
        }
    }

    while (!strstack.empty())
    {
        rpn.push(strstack.top());
        strstack.pop();
    }

    return rpn;
}

/**********************************************
* calculate()
* function that reads the RPN saved and calulates the
* final output of the RPN
* parameter: none
* return: vector<long> - vector of record numbers filtered
********************************************/
vector<long> RPN::calculate()
{
    stack<_operator> token_stack;

    while (!output.empty())
    {
        if (presidence_map[output.front()] == RELATION)
        {
            string anOperator = output.front();
            int status = relation_map[anOperator];

            //grabs top two items when operator is found
            _operator temp1 = token_stack.top();
            token_stack.pop();
            _operator temp2 = token_stack.top();
            token_stack.pop();

            vector<long> storage;

            switch (status)
            {
                case LESSTHAN:
                {
                    //cout << "LE" << endl;
                    storage = less_than(temp2.token_str(), temp1.token_str());
                    break;
                }

                case GREATERTHAN:
                {
                    //cout << "GE" << endl;
                    storage = greater_than(temp2.token_str(), temp1.token_str());
                    break;
                }

                case LESSTHANEQUAL:
                {
                    //cout << "LEQUAL" << endl;
                    storage = less_than_equal(temp2.token_str(), temp1.token_str());
                    break;
                }

                case GREATERTHANEQUAL:
                {
                    //cout << "GEQUAL" << endl;
                    storage = greater_than_equal(temp2.token_str(), temp1.token_str());
                    break;
                }

                case EQUAL:
                {
                    //cout << "EQUAL" << endl;
                    storage = equal_to(temp2.token_str(), temp1.token_str());
                    break;
                }

                default:
                {
                    cout << "ERROR UNKNOWN OPERATOR HAS BEEN READ" << endl;
                    break;
                }
            }

            token_stack.push(_operator(storage, " ", 0));
        }
        else if (presidence_map[output.front()] == AND)
        {
            vector<long> v1 = token_stack.top().get_list();
            token_stack.pop();
            vector<long> v2 = token_stack.top().get_list();
            token_stack.pop();

            vector<long> temp = vector_instersection(v1,v2);

            token_stack.push(_operator(temp, " ", 0));
        }
        else if (presidence_map[output.front()] == OR)
        {
            vector<long> v1 = token_stack.top().get_list();
            token_stack.pop();
            vector<long> v2 = token_stack.top().get_list();
            token_stack.pop();

            vector<long> temp = vector_union(v1, v2);
            token_stack.push(_operator(temp, " ", 0));
        }
        else
        {
            vector<long> temp;
            token_stack.push(_operator(temp,output.front(), 1));
        }
        output.pop();
    }
    return token_stack.top().get_list();
}


/**********************************************
* less_than(string field, string value)
* function that calculates entries in field that are
* less than the value
* parameter: string field - field to check
*            string value - value to compare
* return: vector<long> - vector of record numbers filtered
********************************************/
vector<long> RPN::less_than(string field, string value)
{
    vector<MMap<string, int>> tableIndicies = storedTable.get_indicies();

    int index = storedTable.get_field_index()[field];

    vector<long> recno;

    MMap<string, int>::Iterator it = tableIndicies[index].begin();
    MMap<string, int>::Iterator endit = tableIndicies[index].lower_bound(value);

    while(it != endit)
    {
        for (int i = 0; i < (*it).value_list.size(); i++)
        {
            recno.push_back((*it).value_list[i]);
        }
        it++;
    }
    return recno;
}

/**********************************************
* less_than_equal(string field, string value)
* function that calculates entries in field that are
* less than or equal the value
* parameter: string field - field to check
*            string value - value to compare
* return: vector<long> - vector of record numbers filtered
********************************************/
vector<long> RPN::less_than_equal(string field, string value)
{
    vector<MMap<string, int>> tableIndicies = storedTable.get_indicies();

    int index = storedTable.get_field_index()[field];

    vector<long> recno;

    MMap<string, int>::Iterator it = tableIndicies[index].begin();
    MMap<string, int>::Iterator endit = tableIndicies[index].upper_bound(value);
    while (it != endit)
    {
        for (int i = 0; i < (*it).value_list.size(); i++)
        {
            (*it).value_list[i];
            recno.push_back((*it).value_list[i]);
        }
        it++;
    }
    return recno;
}

/**********************************************
* greater_than(string field, string value)
* function that calculates entries in field that are
* greater than the value
* parameter: string field - field to check
*            string value - value to compare
* return: vector<long> - vector of record numbers filtered
********************************************/
vector<long> RPN::greater_than(string field, string value)
{
    vector<MMap<string, int>> tableIndicies = storedTable.get_indicies();

    int index = storedTable.get_field_index()[field];

    vector<long> recno;

    MMap<string, int>::Iterator it = tableIndicies[index].upper_bound(value);
    MMap<string, int>::Iterator endit = tableIndicies[index].end();

    while (it != endit)
    {
        for (int i = 0; i < (*it).value_list.size(); i++)
        {
            recno.push_back((*it).value_list[i]);
        }
        it++;
    }
    return recno;
}

/**********************************************
* greater_than(string field, string value)
* function that calculates entries in field that are
* greater than or equal to the value
* parameter: string field - field to check
*            string value - value to compare
* return: vector<long> - vector of record numbers filtered
********************************************/
vector<long> RPN::greater_than_equal(string field, string value)
{
    vector<MMap<string, int>> tableIndicies = storedTable.get_indicies();

    int index = storedTable.get_field_index()[field];

    vector<long> recno;

    MMap<string, int>::Iterator endit = tableIndicies[index].end();
    MMap<string, int>::Iterator it = tableIndicies[index].lower_bound(value);
    while (it != endit)
    {
        for (int i = 0; i < (*it).value_list.size(); i++)
        {
            recno.push_back((*it).value_list[i]);
        }
        it++;
    }
    return recno;
}

/**********************************************
* greater_than(string field, string value)
* function that calculates entries in field that are
* equal to the value
* parameter: string field - field to check
*            string value - value to compare
* return: vector<long> - vector of record numbers filtered
********************************************/
vector<long> RPN::equal_to(string field, string value)
{
    vector<MMap<string, int>> tableIndicies = storedTable.get_indicies();

    int index = storedTable.get_field_index()[field];

    vector<long> recno;

    MMap<string, int>::Iterator it = tableIndicies[index].find(value);
    
    if (tableIndicies[index].contains(value))
    {
        for (int i = 0; i < (*it).value_list.size(); i++)
        {
            recno.push_back((*it).value_list[i]);
        }
    }

    return recno;
}


/**********************************************
* build_maps()
* function that builds the necessary for calculations
* parameter: none
* return: none
********************************************/
void RPN::build_maps()
{
    presidence_map.insert("and", AND);
    presidence_map.insert("or", OR);
    presidence_map.insert("<", RELATION);
    presidence_map.insert(">", RELATION);
    presidence_map.insert("<=", RELATION);
    presidence_map.insert(">=", RELATION);
    presidence_map.insert("=", RELATION);
    presidence_map.insert("(", 5);
    presidence_map.insert(")", 10);

    relation_map.insert("<", LESSTHAN);
    relation_map.insert(">", GREATERTHAN);
    relation_map.insert("<=", LESSTHANEQUAL);
    relation_map.insert(">=", GREATERTHANEQUAL);
    relation_map.insert("=", EQUAL);
}