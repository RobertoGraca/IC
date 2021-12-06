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
        try
        {
            if (*perm != 'r' && *perm != 'w')
            {
                throw *perm;
            }
            this->perm = perm;
            this->file = fopen(path, this->perm);
            this->bit_count = 0;

            if (this->file == nullptr)
            {
                throw 0;
            }
        }
        catch (char myException)
        {
            if (myException == *perm)
            {
                cout << "Illegal Permission: " << myException << endl;
                cout << "Only Read (r) and Write (w) are allowed!" << endl;
                exit(1);
            }
        }
        catch (int myException)
        {
            if (myException == 0)
            {
                cout << "File Does Not Exist: " << path << endl;
                cout << "It's not possible to create or locate the specified file!" << endl;
                exit(1);
            }
        }
    }

    bool read_bit()
    {
        if (*this->perm != 'r')
        {
            cout << "Reading Permission Required" << endl;
            exit(2);
        }

        if (this->bit_count == 0)
        {
            this->current_char = fgetc(this->file);
        }

        if (this->current_char == EOF)
        {
            cout << "End of file reached" << endl;
            this->current_char = 0;
            return false;
        }

        buffer.push_back((this->current_char >> (7 - this->bit_count)) & 0x01);
        this->bit_count = (this->bit_count + 1) % 8;

        return true;
    }

    void read_n_bits(int n)
    {
        int i;
        for (i = 0; i < n; i++)
        {
            if (!this->read_bit())
            {
                cout << i << " of " << n << " bits read" << endl;
                break;
            }
        }
    }

    bool write_bit(bool bit)
    {
        if (*this->perm != 'w')
        {
            cout << "Writing Permission Required" << endl;
            exit(2);
        }
        buffer.push_back(bit);

        if (this->can_write())
        {
            char tmp = this->bool_array_to_char();
            return fputc(tmp, this->file) == tmp;
        }
        return true;
    }

    bool write_n_bits(vector<bool> c)
    {
        int n = 0;
        for (auto i = c.begin(); i != c.end(); ++i)
        {
            if (!this->write_bit(*i))
            {
                cout << n << " of " << c.size() << " bits written" << endl;
                break;
            }
            n++;
        }
        return true;
    }

    bool close()
    {
        fclose(this->file);
        return 0;
    }

    void show_buffer()
    {
        int n = 0;
        for (auto i = this->buffer.begin(); i != this->buffer.end(); i++)
        {
            if (n % 8 == 0)
            {
                cout << endl;
            }
            cout << *i << " ";
            n++;
        }
        cout << endl;
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