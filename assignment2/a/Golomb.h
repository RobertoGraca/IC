#include <cmath>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <string>
#include "BitStream.h"
#include <fstream>
using namespace std;

class Golomb
{
private:
    int m, r_bits;
    int id;
    vector<vector<bool>> r_combs; // vector with truncated bits combinations
    inline static int cont = 0;

public:
    Golomb(const int m)
    {
        this->m = m;
        this->id = cont++;
        this->r_bits = floor(log2(m)); // Power of 2 lower than m

        // Creation of all the possibilities for the "r" value
        // Numbers that are going to be written with one more bit
        for (int i = 0; i < ((this->m - pow(2, r_bits)) * 2); i++)
        {
            int j = pow(2, this->r_bits + 1) - i - 1;
            vector<bool> x;
            this->r_combs.insert(this->r_combs.cbegin(), x);
            while (j != 0)
            {
                this->r_combs[0].insert(this->r_combs[0].cbegin(), j % 2 == 0 ? false : true);
                j /= 2;
            }
            for (int i = this->r_combs[0].size(); i <= this->r_bits; i++)
            {
                this->r_combs[0].insert(this->r_combs[0].cbegin(), false);
            }
        }

        // Numbers that are going to be written with one less bit
        for (int i = this->m - this->r_combs.size() - 1; i >= 0; i--)
        {
            if (this->r_combs.size() >= m)
                break;

            int j = i;
            vector<bool> x;
            this->r_combs.insert(this->r_combs.cbegin(), x);
            while (j != 0)
            {
                this->r_combs[0].insert(this->r_combs[0].cbegin(), j % 2 == 0 ? false : true);
                j /= 2;
            }
            for (int i = this->r_combs[0].size(); i < this->r_bits; i++)
            {
                this->r_combs[0].insert(this->r_combs[0].cbegin(), false);
            }
        }

        /*for (auto i = this->r_combs.cbegin(); i != this->r_combs.cend(); i++)
        {
            std::ostringstream oss;
            std::copy((*i).begin(), (*i).end(),
                      std::ostream_iterator<int>(oss, " "));
            cout << oss.str() << endl;
        }*/
    }

    void encode(vector<int> nums_to_encode)
    {
        string filename = "encoded" + to_string(this->id) + ".bits";
        BitStream enc{filename.c_str(), "w"};

        // vector<bool> show;

        for (auto elem = nums_to_encode.cbegin(); elem != nums_to_encode.cend(); elem++)
        {
            int n = *elem;
            bool signal = n < 0;
            if (signal)
                n = -n;
            int q = floor(n / this->m);
            int r = n % this->m;

            vector<bool> num(q, false);
            /*for (int i = 0; i < q; i++)
            {
                show.push_back(false);
            }*/
            num.push_back(true);
            // show.push_back(true);

            enc.write_n_bits(num);

            for (auto i = r_combs[r].cbegin(); i != r_combs[r].cend(); i++)
            {
                enc.write_bit(*i);
                // show.push_back(*i);
            }
            if (n != 0)
                enc.write_bit(signal);
        }

        enc.close();

        /*while (show.size() % 8 != 0) // flush the buffer
        {
            show.push_back(false);
        }

        int cont = 0;
        cout << endl << "Encoder" << endl;
        for (auto i = show.cbegin(); i != show.cend(); i++)
        {
            if (cont++ % 8 == 0)
                cout << endl;
            cout << *i << " ";
        }

        cout << endl;
        print("buffer size = ", show.size());
        show.erase(show.cbegin(), show.cend());*/
    }

    vector<int> decode()
    {
        string filename = "encoded" + to_string(this->id) + ".bits";
        BitStream enc{filename.c_str(), "r"};
        int bit_count = 0;

        vector<int> nums;

        while (!enc.read_n_bits(8))
        {
        }
        // cout << endl << "Decoder" << endl;
        //  enc.show_buffer();
        //  print("buffer size = ", enc.buffer.size());

        while (1)
        {
            int q = 0, r = 0;

            // reads the number of initial "0"
            for (auto i = enc.buffer.cbegin() + bit_count; i != enc.buffer.cend(); i++)
            {
                if (!*i)
                    q++;
                else
                    break;
            }
            bit_count += (q + 1);

            // reads the next r_bits
            vector<bool> tmp;
            for (auto i = enc.buffer.cbegin() + bit_count; i != enc.buffer.cbegin() + bit_count + this->r_bits; i++)
            {
                tmp.push_back(*i);
            }

            // if the number read is smaller than 2^(r_bits+1) - m, reads one more bit
            int n = this->bool_array_to_char(tmp);
            if (n >= (pow(2, this->r_bits + 1) - this->m) && n != this->m)
            {
                tmp.push_back(*(enc.buffer.cbegin() + bit_count + this->r_bits));
            }
            for (auto i = this->r_combs.cbegin(); i != this->r_combs.cend(); i++)
            {
                if (*i == tmp)
                    break;
                r++;
            }

            bit_count += this->r_combs[r].size();

            if (bit_count > enc.buffer.size())
                break;

            bool signal = 0;
            if (q != 0 || r != 0)
            {
                signal = enc.buffer.at(bit_count + 1);
                bit_count++;
            }

            nums.push_back(signal ? -1 * ((this->m * q) + r) : ((this->m * q) + r));
        }

        enc.close();
        return nums;
    }

    // converts the binary combination in vector x to decimal
    int bool_array_to_char(vector<bool> x)
    {
        int n = 0, i = 0;
        for (auto it = x.cbegin(); it != x.cend(); ++it)
        {
            if (*it)
            {
                n |= (1 << (x.size() - 1 - i));
            }
            i++;
        }
        return n;
    }

    void print(string s, int x)
    {
        cout << s << x << endl;
    }

    template <typename T>
    void show_vector(vector<T> x)
    {
        int n = 0;
        for (auto i = x.cbegin(); i != x.cend(); i++)
        {
            if (n++ % 8 == 0)
                cout << endl;
            cout << *i << ' ';
        }
        cout << endl;
    }

    // return the number of characters used to encode the given message
    int get_filename_num_chars()
    {
        ifstream file("encoded" + to_string(this->id) + ".bits");
        int char_count = 0;
        char c;
        while (!file.eof())
        {
            file >> c;
            char_count++;
        }
        return char_count;
    }
};