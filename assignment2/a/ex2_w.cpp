#include "BitStream.h"

int main(int argc, char** argv)
{
    if (argc > 2){
        cout << "USAGE: ./ex2_w [file_to_be_written]" << endl;
        exit(1);
    }

    const char *filename;
    if (argc == 2){
        filename = argv[1];
    } else{
        // default filename
        filename = "write.bits";
    }

    const char *perm = "w";
    BitStream bs(filename, perm);

    vector<bool> vector{0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0};

    // write '#' -> ASCII code 00100011
    //   and '@' -> ASCII code 01000000
    bs.write_n_bits(vector);

    /* OR: 
    bs.write_bit(0);
    bs.write_bit(0);
    bs.write_bit(1);
    bs.write_bit(0);
    bs.write_bit(0);
    bs.write_bit(0);
    bs.write_bit(1);
    bs.write_bit(1);

    bs.write_bit(0);
    bs.write_bit(1);
    bs.write_bit(0);
    bs.write_bit(0);
    bs.write_bit(0);
    bs.write_bit(0);
    bs.write_bit(0);
    bs.write_bit(0);
    */

    bs.close();

    return 0;
}