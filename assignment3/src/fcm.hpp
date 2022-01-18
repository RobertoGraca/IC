#include <iostream>
#include <map>
#include <set>
#include <fstream>
#include <algorithm>
#include <vector>
#include <assert.h>
#include <cmath>
using namespace std;

class FCM
{
private:
    int k;                             // model order (context size)
    float alpha;                       // smoothing parameter
    map<string, map<char, int>> index; // saves the number of symbol ocurrences for a context
    map<string, int> count_ctx;        // counts the numbers of ocurrences of a context
    set<char> alphabet;                // characters present in a file
    long num_chars;
    vector<char> text;

public:
    FCM(int k, int alpha)
    {
        assert(k > 0);
        assert(alpha > 0 && alpha <= 100);

        this->k = k;
        this->alpha = (float)alpha / 100.0;
    }

    // fills the map with the number of symbols associated with a context
    void read_file(string path)
    {
        this->index.erase(this->index.cbegin(), this->index.cend());
        this->count_ctx.erase(this->count_ctx.cbegin(), this->count_ctx.cend());
        this->alphabet.erase(this->alphabet.cbegin(), this->alphabet.cend());
        this->num_chars = 0;
        this->text.erase(this->text.cbegin(), this->text.cend());

        ifstream ifs(path);
        if (!ifs.good())
        {
            cout << "File \"" << path << "\" could not be located." << endl;
            exit(1);
        }
        this->get_alphabet_from_file(path);
        for (int i = 0; i < this->num_chars - this->k; i++)
        {
            string context = this->make_context(i);
            char symbol = this->text[i + this->k];
            this->add_to_index(context, symbol);
        }
    }

    // calculates the model entropy based on the weighted probability of the appearance of a context
    float get_model_entropy()
    {
        float entropy = 0;
        for (auto i = this->index.cbegin(); i != this->index.cend(); i++)
        {
            entropy += ((float)this->get_context_entropy(i->first) * (float)this->get_context_probability(i->first));
        }
        return entropy;
    }

    // prints the information on the index dictionary
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

    // counts the number of ocurrences of symbol after a specific context
    void add_to_index(string context, char symbol)
    {
        if (this->index.count(context) == 0)
        {
            map<char, int> tmp;
            tmp.emplace(symbol, 1);

            this->index.emplace(context, tmp);
            this->count_ctx.emplace(context, 1);
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
            this->count_ctx[context]++;
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

    // gets every character present in a file
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

    // calculates the entropy of a specific symbol for a specific context
    float get_symbol_entropy(string ctx, char symbol)
    {
        float prob = this->get_symbol_probability(ctx, symbol);
        return prob * log2(prob);
    }

    // calculates the apparition of a specific symbol for a specific context
    float get_symbol_probability(string ctx, char symbol)
    {
        return ((float)this->index[ctx][symbol] + (float)this->alpha) / ((float)this->count_ctx[ctx] + ((float)this->alpha * (float)this->alphabet.size()));
    }

    // counts the number of contexts (repeated and not repeated) in a text file
    int get_total_ocurrences()
    {
        int cont = 0;
        for (auto i = this->count_ctx.cbegin(); i != this->count_ctx.cend(); i++)
        {
            cont += i->second;
        }
        return cont;
    }

    // calculates the entropy of a specific context of the model
    float get_context_entropy(string ctx)
    {
        map<char, int> tmp = this->index[ctx];
        float entropy = 0;
        for (auto i = tmp.cbegin(); i != tmp.cend(); i++)
        {
            entropy += (float)this->get_symbol_entropy(ctx, i->first);
        }
        cout << entropy << endl;
        return -entropy;
    }

    // calculates the probability of a specific context of the model
    float get_context_probability(string ctx)
    {
        return (float)this->count_ctx[ctx] / (float)this->get_total_ocurrences();
    }

    void show_ctx_ocurrences()
    {
        for (auto i = this->count_ctx.cbegin(); i != this->count_ctx.cend(); i++)
        {
            cout << i->first << " - " << i->second << endl;
        }
    }
};