#include <iostream>
#include <map>
#include <set>
#include <fstream>
#include <algorithm>
#include <vector>
#include <assert.h>
using namespace std;

class FCM
{
private:
    int k;                             // model order (context size)
    int alpha;                         // smoothing parameter
    map<string, map<char, int>> index; // saves the number of symbol ocurrences for a context
    set<char> alphabet;                // characters present in a file
    long num_chars;
    vector<char> text;

public:
    FCM(int k, int alpha)
    {
        assert(k > 0);
        assert(alpha > 0 && alpha <= 100);

        this->k = k;
        this->alpha = alpha / 100;
        this->num_chars = 0;
    }

    void read_file(string path)
    {
        this->get_alphabet_from_file(path);
        ifstream ifs(path);
        for (int i = 0; i < this->num_chars - this->k; i++)
        {
            string context = this->make_context(i);
            char symbol = this->text[i + this->k];
            this->add_to_index(context, symbol);
        }
    }

    // counts the number of ocurrences of symbol after a specific context
    void add_to_index(string context, char symbol)
    {
        if (this->index.count(context) == 0)
        {
            map<char, int> tmp;
            tmp.emplace(symbol, 1);

            this->index.emplace(context, tmp);
        }
        else
        {
            if (this->index[context].count(symbol) == 0)
            {
                this->index[context].emplace(symbol, 1);
            }
            else
            {
                this->index[context][symbol]++;
            }
        }
    }

    // creates a new context based on the given text
    string make_context(int x)
    {
        string s;
        for (int i = x; i < x + this->k; i++)
        {
            s += this->text[i];
        }
        return s;
    }

    // Gets every character present in a file
    void get_alphabet_from_file(string path)
    {
        ifstream ifs(path);
        while (!ifs.eof())
        {
            char x = ifs.get();

            this->alphabet.insert(x);
            this->text.push_back(x);
            this->num_chars++;
        }
        if (find(this->alphabet.cbegin(), this->alphabet.cend(), EOF) != this->alphabet.cend())
        {
            this->alphabet.erase(EOF);
            this->text.erase(this->text.cend() - 1, this->text.cend() - 1);
            this->num_chars--;
        }
    }

    void show_index()
    {
        for (auto i = this->index.cbegin(); i != this->index.cend(); i++)
        {
            cout << "Context = " << i->first << endl
                 << endl;
            for (auto j = i->second.cbegin(); j != i->second.cend(); j++)
            {
                if (j->first == '\n')
                    cout << "\\n - " << j->second << endl;
                else if (j->first == ' ')
                    cout << "\\w - " << j->second << endl;
                else
                    cout << j->first << " - " << j->second << endl;
            }
        }
    }
};