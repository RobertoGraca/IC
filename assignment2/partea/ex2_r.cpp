#include "BitStream.h"

int main()
{
    const char *filename = "read.bits";
    const char *perm = "r";
    BitStream bs(filename, perm);

    bs.read_bit();
    bs.read_bit();
    bs.read_bit();
    bs.read_bit();
    bs.read_bit();
    bs.read_bit();
    bs.read_bit();
    bs.read_bit();
    cout << bs.buffer;
    bs.close();
    // read 01111011 -> ASCII code for '{'
}