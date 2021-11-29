#include <iostream>
using namespace std;

class BitStream
{
public:
    bool *buffer;
    FILE *file;
    const char *perm;
    int bit_count;
    char current_char;

    BitStream(const char *path, const char *perm)
    {
        if (*perm != 'r' && *perm != 'w')
        {
            throw "Illegal Permission Exception!";
        }
        this->perm = perm;
        this->file = fopen(path, this->perm);
        this->bit_count = 0;
    }

    bool read_bit()
    {
        if (this->bit_count == 0)
        {
            this->current_char = fgetc(this->file);
        }

        if (this->current_char == EOF)
        {
            return false;
        }

        *buffer = (this->current_char >> this->bit_count) & 0x01;
        buffer++;
        this->bit_count = (this->bit_count + 1) % 8;

        return true;
    }

    bool *read_n_bits(int n)
    {
        bool read_n[n];
        int i;
        for (i = 0; i < n; i++)
        {
            read_n[i] = this->read_bit();
        }
        return read_n;
    }

    bool write_bit(bool bit)
    {
        *buffer = bit;
        buffer++;

        if (this->can_write())
        {
            fputc(this->bool_array_to_char(), this->file);
            return true;
        }
        return false;
    }

    bool write_n_bits(bool *c)
    {
        bool read;
        while (*c != '\0')
        {
            read = this->write_bit(*c);
            if (!read)
                return false;
        }
        return true;
    }

    bool close()
    {
        fclose(this->file);
        return 0;
    }

    bool buffer_is_empty()
    {
        return !this->buffer;
    }

    bool can_write()
    {
        return ((sizeof(this->buffer) / this->buffer[0]) >= 8);
    }

    char bool_array_to_char()
    {
        int n = 0;
        int i;
        for (i = 0; i < 8; i++)
        {
            if (this->buffer[i])
            {
                n |= (1 << (8 - i));
            }
        }
        return (char)n;
    }
};