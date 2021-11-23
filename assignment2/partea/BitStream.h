#include <iostream>
using namespace std;

class BitStream
{
public:
    bool buffer[8];
    FILE *file;

    BitStream(const char *path, const char *perm)
    {
        this->file = fopen(path, perm);
    }

    bool read_bit()
    {
        char c = fgetc(this->file);
        return (c >> 0) & 1;
    }

    bool* read_n_bits(int n)
    {
        // Algo de errado não está certo aqui...
        char c = fgetc(this->file);
        for(int i = 0; i<n; i++){
            buffer[i] = (c >> i) & 1;
            cout << buffer[i];
        }
        return buffer;
    }

    bool write_bit(bool bit)
    {
        // TODO
        return 0;
    }

    bool write_n_bits(int b)
    {
        // TODO
        return 0;
    }

    bool close()
    {
        fclose(this->file);
        return 0;
    }
};