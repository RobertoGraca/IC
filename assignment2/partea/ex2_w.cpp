#include "BitStream.h"

int main()
{
    const char *filename = "write.bits";
    const char *perm = "w";
    BitStream bs(filename, perm);
    vector<bool> vectLeftBrack{0, 1, 1, 1, 1, 0, 1, 1};

    bs.write_n_bits(vectLeftBrack);
    bs.write_bit(0);
    bs.write_bit(0);
    bs.write_bit(1);
    bs.write_bit(0);
    bs.write_bit(0);
    bs.write_bit(0);
    bs.write_bit(1);
    bs.write_bit(1);

    bs.close();
    // write 00100011 -> ASCII code for '#'
}