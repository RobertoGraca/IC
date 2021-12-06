#include <iostream>
#include <vector>
using namespace std;

class BitStream
{
public:
    vector<bool> buffer;
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

        buffer.push_back((this->current_char >> this->bit_count) & 0x01);
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

        buffer.push_back(bit);

        if (this->can_write())
        {
            return fputc(this->bool_array_to_char(), this->file);
        }
        return false;
    }

    bool write_n_bits(vector<bool> c)
    {
        for (auto i = c.begin(); i != c.end(); ++i)
        {
            bool read = this->write_bit(*i);
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
        return this->buffer.empty();
    }

    bool can_write()
    {
        return this->buffer.size() >= 8;
    }

    char bool_array_to_char()
    {
        int n = 0, i = 0;
        for (auto it = this->buffer.begin(); it != this->buffer.end(); ++it)
        {
            if (*it)
            {
                n |= (1 << (7 - i));
            }
            i++;
        }
        this->buffer.erase(this->buffer.begin(), this->buffer.begin() + 8);
        return (char)n;
    }
};