#ifndef Z_WORK_REPORT_H
#define Z_WORK_REPORT_H

/*
    Features:
        -Not Implemented: none

        -Implemented: program that reads file,converts into blocks, the blocks to strings and
                        generates tokens from the strings

        -Partly implemented: none

    Bugs     : when the tokenizer reads a null character, instead of keeping the state to 0,
               the program will set the state to an Alphabet no matter the previous state it
               was in.

               occasionally it will read a special character as an ALPHA

    Reflections:
        -I could find the source of the special characters being read as an ALPHA and fix it

*/

#endif // Z_WORK_REPORT_H
