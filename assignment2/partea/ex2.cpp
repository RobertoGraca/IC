#include "BitStream.h"

int main(){
    BitStream bs("test.bits","w");

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