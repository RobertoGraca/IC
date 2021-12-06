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
    // debug
    // ##### TODO: currently printing backwards #####
    for (auto i = bs.buffer.begin(); i != bs.buffer.end(); ++i)
        std::cout << *i << ' ';
    bs.close();
    // read 01111011 -> ASCII code for '{'
}