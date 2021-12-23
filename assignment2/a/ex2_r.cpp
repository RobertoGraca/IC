#include "BitStream.h"

int main(int argc, char** argv)
{
    if (argc > 2){
        cout << "USAGE: ./ex2_r [file_to_be_read]" << endl;
        exit(1);
    }

    const char *filename;
    if (argc == 2){
        filename = argv[1];
    } else{
        // default filename
        filename = "read.bits";
    }

    const char *perm = "r";
    BitStream bs(filename, perm);

    // read 01111011 -> ASCII code for '{'
    //  and 00100100 -> ASCII code for '$'
    bs.read_n_bits(800);

    /* OR: 
    bs.read_bit();
    bs.read_bit();
    bs.read_bit();
    bs.read_bit();
    bs.read_bit();
    bs.read_bit();
    bs.read_bit();
    bs.read_bit();

    bs.read_bit();
    bs.read_bit();
    bs.read_bit();
    bs.read_bit();
    bs.read_bit();
    bs.read_bit();
    bs.read_bit();
    bs.read_bit();
    */
    
    bs.show_buffer();
    bs.close();
    
    return 0;
}