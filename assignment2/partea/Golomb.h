#include <cmath>
#include <vector>
#include <algorithm>
#include <algorithm>
#include <sstream>
#include <iterator>
using namespace std;

class Golomb
{
public:
    int m;
    vector<vector<bool>> r_combs; // vector with truncated bits combinations

    Golomb(const int m)
    {
        this->m = m;
        int r_bits = floor(log2(m));                               // Power of 2 lower than m
        for (int i = 0; i < ((this->m - pow(2, r_bits)) * 2); i++) // Number of numbers that are gonna have one more bit
        {
            int j = pow(2, r_bits + 1) - i - 1;
            vector<bool> x;
            this->r_combs.insert(this->r_combs.cbegin(), x);
            while (j != 0)
            {
                this->r_combs[0].insert(this->r_combs[0].cbegin(), j % 2 == 0 ? false : true);
                j /= 2;
            }
            for (int i = this->r_combs[0].size(); i <= r_bits; i++)
            {
                this->r_combs[0].insert(this->r_combs[0].cbegin(), false);
            }
        }

        for (int i = this->m - this->r_combs.size() - 1; i >= 0; i--) // Number of numbers that are gonna have one less bit
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
            for (int i = this->r_combs[0].size(); i < r_bits; i++)
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

    vector<bool> encode(int n)
    {
        int q = floor(n / this->m);
        int r = n % this->m;
        vector<bool> encoded_num;

        for (int i = 0; i < q; i++)
        {
            encoded_num.push_back(false);
        }
        encoded_num.push_back(true);

        for (auto i = r_combs[r].cbegin(); i != r_combs[r].cend(); i++)
        {
            encoded_num.push_back(*i);
        }

        return encoded_num;
    }

    int decode(vector<bool> x)
    {
        int q = 0;
        for (auto i = x.cbegin(); i != x.cend(); i++)
        {
            if (!*i)
                q++;
            else
                break;
        }
        int r = 0;
        vector<bool> tmp;
        for (auto i = x.cend() - 1; i != x.cbegin() + q; i--) // TODO may need to be changed to accomodate truncated binary
        {
            tmp.insert(tmp.cbegin(), *i);
        }
        for (auto i = this->r_combs.cbegin(); i != this->r_combs.cend(); i++)
        {
            if (*i == tmp)
                break;
            r++;
        }
        return (this->m * q) + r;
    }
};