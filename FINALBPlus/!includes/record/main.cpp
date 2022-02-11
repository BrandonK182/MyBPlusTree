#include <iostream>
#include <fstream>
using namespace std;

//utility functions
bool file_exists(const char filename[]);

void open_fileRW(fstream& f, const char filename[]) throw(char*);
void open_fileW(fstream& f, const char filename[]);


class Record{
public:
    //when you construct a Record, it's either empty or it
    //  contains a word
    Record(){
        _record[0] = NULL;
        recno = -1;
    }

    Record(char str[]){
        strncpy(_record, str, MAX);
    }
    Record(string s){
        strncpy(_record, s.c_str(), MAX);
    }
    long write(fstream& outs);              //returns the record number
    long read(fstream& ins, long recno);    //returns the number of bytes
                                            //      read = MAX, or zero if
                                            //      read passed the end of file


    friend ostream& operator<<(ostream& outs,
                               const Record& r);
private:
    static const int MAX = 10;
    static const int MAX_ROWS = 10;
    static const int MAX_COLS = 10;
    int recno;
    char _record[MAX_ROWS][MAX_COLS];
};

long Record::write(fstream &outs){

    //write to the end of the file.
    outs.seekg(0, outs.end);


    long pos = outs.tellp();    //retrieves the current position of the
                                //      file pointer

    //outs.write(&record[0], sizeof(record));
    outs.write(_record, sizeof(_record));

    return pos/sizeof(_record);  //record number
}

long Record::read(fstream &ins, long recno){
    long pos= recno * sizeof(_record);
    ins.seekg(pos, ios_base::beg);


    ins.read(_record, sizeof(_record));
    //don't you want to mark the end of  the cstring with null?
    //_record[] => [zero'\0'trash trash trash trash]
    //don't need the null character, but for those of us with OCD and PTSD:
    _record[ins.gcount()] = '\0';
    return ins.gcount();

}
ostream& operator<<(ostream& outs,
                    const Record& r){
    outs<<r._record;
    return outs;
}
//[    |    |     |    |    |     |]
//-------------------------------------------------
bool file_exists(const char filename[]){
    const bool debug = false;
    fstream ff;
    ff.open (filename,
        std::fstream::in | std::fstream::binary );
    if (ff.fail()){
        if (debug) cout<<"file_exists(): File does NOT exist: "<<filename<<endl;
        return false;
    }
    if (debug) cout<<"file_exists(): File DOES exist: "<<filename<<endl;
    ff.close();
    return true;
}

void open_fileRW(fstream& f, const char filename[]) throw(char*){
    const bool debug = false;
    //openning a nonexistent file for in|out|app causes a fail()
    //  so, if the file does not exist, create it by openning it for
    //  output:
    if (!file_exists(filename)){
        //create the file
        f.open(filename, std::fstream::out|std::fstream::binary);
        if (f.fail()){
            cout<<"file open (RW) failed: with out|"<<filename<<"]"<<endl;
            throw("file RW failed  ");
        }
        else{
            if (debug) cout<<"open_fileRW: file created successfully: "<<filename<<endl;
        }
    }
    else{
        f.open (filename,
            std::fstream::in | std::fstream::out| std::fstream::binary );
        if (f.fail()){
            cout<<"file open (RW) failed. ["<<filename<<"]"<<endl;
            throw("file failed to open.");
        }
    }

}
void open_fileW(fstream& f, const char filename[]){
    f.open (filename,
            std::fstream::out| std::fstream::binary );
    if (f.fail()){
        cout<<"file open failed: "<<filename<<endl;
        throw("file failed to open.");
    }

}

int main(int argc, char *argv[])
{
    cout <<endl<<endl<<endl<< "================================" << endl;
    string list[10] = {"zero",
                       "one",
                       "two",
                       "three",
                       "four",
                       "five",
                       "six",
                       "seven",
                       "eight",
                       "nine"};

    for (int i = 0; i< 10; i++){
        cout<<list[i]<<endl;
    }

    fstream f;
    //reset the file:
    open_fileW(f, "record_list.bin");
    for (int i= 0; i<10; i++){
        Record r(list[i]);      //filling the envelop
        long recno = r.write(f);
        cout<<"["<<list[i] <<"]"<< " was written into file as record: "<<recno<<endl;
    }
    f.close();



    Record r;
    //open the file for reading and writing.
    open_fileRW(f, "record_list.bin" );
    r.read(f, 3);       //empty envelop to be filled by the Record object
    cout<<"record 3: "<<r<<endl;    //insertion operator of the Record object
    r.read(f, 6);
    cout<<"record 6: "<<r<<endl;

    //reading passed the end of file:
    long bytes = r.read(f, 19);
    cout<<"number of bytes read: "<<bytes<<endl;

    cout <<endl<<endl<<endl<< "================================" << endl;
    return 0;
}
