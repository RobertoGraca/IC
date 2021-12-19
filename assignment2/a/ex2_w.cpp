#include "BitStream.h"

int main()
{
    const char *filename = "write.bits";
    const char *perm = "w";
    BitStream bs(filename, perm);
    vector<bool> vectLeftBrack{0, 1, 1, 1, 1, 0, 1, 1};
    vector<bool> vectOmega{1, 1, 1, 0, 1, 0, 1, 0};
    vector<bool> vect0(8, 0);
    vector<bool> vect1{1, 0, 1, 0, 0, 0, 0, 0};
    vector<bool> vect2{0, 0, 0, 1, 1, 0, 0, 0};
    vector<bool> vect3{0, 0, 0, 0, 0, 0, 1, 1};
    vector<bool> vect4{1, 0, 0, 0, 0, 0, 0, 0};

    bs.write_n_bits(vect0);
    bs.write_n_bits(vect0);
    bs.write_n_bits(vect0);
    bs.write_n_bits(vect1);
    bs.write_n_bits(vect0);
    bs.write_n_bits(vect0);
    bs.write_n_bits(vect2);
    bs.write_n_bits(vect0);
    bs.write_n_bits(vect0);
    bs.write_n_bits(vect3);
    bs.write_n_bits(vect4);
    /*bs.write_n_bits(vectOmega);
    bs.write_bit(0);
    bs.write_bit(0);
    bs.write_bit(1);
    bs.write_bit(0);
    bs.write_bit(0);
    bs.write_bit(0);
    bs.write_bit(1);
    bs.write_bit(1);*/

    bs.close();
    // write 00100011 -> ASCII code for '#'
}