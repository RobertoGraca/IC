#include "BitStream.h"

int main()
{
    const char *filename = "write.bits";
    const char *perm = "r";
    BitStream bs(filename, perm);

    bs.read_n_bits(100);
    for (int i = 0; i < 8; i++)
    {
        bs.read_bit();
    }
    bs.show_buffer();
    bs.close();
    // read 01111011 -> ASCII code for '{'
}