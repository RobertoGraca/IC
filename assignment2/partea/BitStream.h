#include <iostream>
using namespace std;

class BitStream {
    public:
        bool buffer[8];
        FILE* file;
        bool write_bit(bool bit);
        //bool write_n_bits(bool bit);
        bool read_bit();
        //char read_n_bits();
        bool close();

    BitStream(char* path, char* perm) {
        this->file = fopen(path, perm);
    }

    bool BitStream::read_bit() {
        // ...
        return 0;
    }

    bool BitStream::write_bit(bool bit) {
        // ...
        return 0;
    }

    bool BitStream::close() {
        fclose(this->file);
        return 0;
    }
};