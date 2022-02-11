#pragma once
#ifndef RECORD_H
#define RECORD_H

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <ios>
#include "../token/token.h"

using namespace std;

class Record : public Token
{
public:
    //when you construct a Record, it's either empty or it
    //  contains a word
    Record()
    {
        _record[0][0] = NULL;
        recno = -1;
        field_num = 0;
    }

    Record(const vector<string>& v)
    {
        for (int i = 0; i < MAX_ROW; i++) {
            _record[i][0] = NULL;
        }

        for (int i = 0; i < v.size(); i++) {
            strncpy(_record[i], v[i].c_str()+'\0', MAX_COL);
        }
        recno = -1;
        field_num = v.size();
    }
    
    long write(fstream& outs) {         //returns the record number

        //write to the end of the file.
        outs.seekg(0, outs.end);


        long pos = outs.tellp();    //retrieves the current position of the
                                    //      file pointer

        //outs.write(&record[0], sizeof(record));
        outs.write(&_record[0][0], sizeof(_record));

        recno++;

        return pos / sizeof(_record);  //record number
    }    

    long read(fstream& ins, long recno) {
        long pos = recno * sizeof(_record);
        ins.seekg(pos, ios_base::beg);


        ins.read(&_record[0][0], sizeof(_record));
        //don't you want to mark the end of  the cstring with null?
        //_record[] => [zero'\0'trash trash trash trash]
        //don't need the null character, but for those of us with OCD and PTSD:
        return ins.gcount();
    }                                       //returns the number of bytes
                                            //      read = MAX, or zero if
                                            //      read passed the end of file


    friend ostream& operator<<(ostream& outs,
        const Record& r) {
        char c[MAX_COL];
        for (int i = 0; i < r.field_num; i++) {
            strncpy(c, r.get_record()[i].c_str()+'\0', MAX_COL);
            outs.width(15);
            outs << c;
        }
        return outs;
    }

    vector<string> get_record()
    {
        vector<string> vec;
        string s;
        for (int i = 0; i < field_num; i++)
        {
            s = "";

            //gets one word
            for (int k = 0; k < MAX_COL; k++)
            {
                s = s + _record[i][k];
            }
            vec.push_back(s);
        }
        return vec;
    }

    const vector<string> get_record() const
    {
        vector<string> vec;
        string s;
        for (int i = 0; i < field_num; i++)
        {
            s = "";

            //gets one word
            for (int k = 0; k < MAX_COL; k++)
            {
                s = s + _record[i][k];
            }

            vec.push_back(s);
        }
        return vec;
    }

    vector<string> filter(vector<int> indicies)
    {
        vector<string> vec;
        string s;
        int ind;
        for (int i = 0; i < indicies.size(); i++)
        {
            s = "";
            ind = indicies[i];
            //gets one word
            for (int k = 0; k < MAX_COL; k++)
            {
                s = s + _record[ind][k];
            }

            vec.push_back(s);
        }
        return vec;
    }

    int field_num;
    static const int MAX_ROW = 20;
    static const int MAX_COL = 20;

private:
    int recno;
    
    char _record[MAX_ROW][MAX_COL];
};

//utility functions
//[    |    |     |    |    |     |]
//-------------------------------------------------
inline bool file_exists(const char filename[]) {
    const bool debug = false;
    fstream ff;
    ff.open(filename,
        std::fstream::in | std::fstream::binary);
    if (ff.fail()) {
        if (debug) cout << "file_exists(): File does NOT exist: " << filename << endl;
        return false;
    }
    if (debug) cout << "file_exists(): File DOES exist: " << filename << endl;
    ff.close();
    return true;
}

inline void open_fileRW(fstream& f, const char filename[]) throw(char*) {
    const bool debug = false;
    //openning a nonexistent file for in|out|app causes a fail()
    //  so, if the file does not exist, create it by openning it for
    //  output:
    if (!file_exists(filename)) {
        //create the file
        f.open(filename, std::fstream::out | std::fstream::binary);
        if (f.fail()) {
            cout << "file open (RW) failed: with out|" << filename << "]" << endl;
            throw("file RW failed  ");
        }
        else {
            if (debug) cout << "open_fileRW: file created successfully: " << filename << endl;
        }
    }
    else {
        f.open(filename,
            std::fstream::in | std::fstream::out | std::fstream::binary);
        if (f.fail()) {
            cout << "file open (RW) failed. [" << filename << "]" << endl;
            throw("file failed to open.");
        }
    }
}

inline void open_fileW(fstream& f, const char filename[]) {
    f.open(filename,
        std::fstream::out | std::fstream::binary);
    if (f.fail()) {
        cout << "file open failed: " << filename << endl;
        throw("file failed to open.");
    }
}

#endif