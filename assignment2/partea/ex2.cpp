#include "BitStream.h"

int main()
{
    const char *filename = "test.bits";
    const char *perm = "w";
    BitStream bs(filename, perm);

    bs.write_bit(0);
    bs.write_bit(0);
    bs.write_bit(1);
    bs.write_bit(0);
    bs.write_bit(0);
    bs.write_bit(0);
    bs.write_bit(1);
    bs.write_bit(1);
    bs.close();
    // write 00100011 -> ASCII code for #
}