#include <iostream>
#include <map>
#include <set>
#include <fstream>
#include <algorithm>
#include <vector>
#include <assert.h>
#include <cmath>
#include <chrono>
using namespace std;

class FCM
{
private:
    int k;                               // model order (context size)
    float alpha;                         // smoothing parameter
    map<string, map<string, int>> index; // saves the number of symbol ocurrences for a context
    map<string, int> count_ctx;          // counts the numbers of ocurrences of a context
    set<string> alphabet;                // has the charcters present in a file
    long num_chars;
    vector<string> text;

public:
    FCM(int k, float alpha)
    {
        assert(k > 0);
        assert(alpha > 0 && alpha <= 100);

        this->k = k;
        this->alpha = (float)alpha / 100.0;
    }

    // fills the map with the ocurrences of symbols associated with a context
    void read_file(string path)
    {
        ifstream ifs(path);
        if (!ifs.good())
        {
            cout << "File \"" << path << "\" could not be located." << endl;
            exit(1);
        }

        long i = -1;
        auto start = chrono::high_resolution_clock::now();
        while (!ifs.eof())
        {
            char x = ifs.get();

            if (x == '\n' || x == EOF || x == '\t')
                continue;

            string character = "";
            character += x;

            switch ((int)x)
            {
            case -17:
                x = ifs.get();
                character += x;
                x = ifs.get();
                character += x;
                break;
            case -30:
                x = ifs.get();
                character += x;
                x = ifs.get();
                character += x;
                break;
            default:
                if ((int)x > -1)
                    break;
                x = ifs.get();
                character += x;
                break;
            }

            // reads multi-character characters
            /*
            string character = "";
            character += x;
            if ((int)x == -61)
            {
                x = ifs.get();
                character += x;
            }
            else if ((int)x < 0)
            {
                char peek = ifs.peek();
                while ((int)peek != -61 && (int)peek < -1)
                {
                    x = ifs.get();
                    character += x;
                    peek = ifs.peek();
                }
            } */

            this->alphabet.insert(character);
            this->text.push_back(character);
            this->num_chars++;

            if (this->text.size() < this->k + 1)
                continue;
            string context = this->make_context(++i);
            string symbol = this->text[i + this->k];
            this->add_to_index(context, symbol);
        }
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
        cout << "Reading and indexing the file took " << duration.count() << " seconds." << endl;
        cout << "Index has " << this->index.size() << " entries." << endl;

        this->text.erase(this->text.cbegin(), this->text.cend());
    }

    // calculates the model entropy based on the weighted probability of the appearance of a context
    float get_model_entropy()
    {
        auto start = chrono::high_resolution_clock::now();
        float entropy = 0;
        for (auto i = this->index.cbegin(); i != this->index.cend(); i++)
        {
            // cout << "\ncontext = " << i->first << endl;
            entropy += ((float)this->get_context_entropy(i->first) * (float)this->get_context_probability(i->first));
        }
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
        cout << "Calculating entropy of the model took " << duration.count() << " seconds." << endl;
        return entropy;
    }

    // writes the model context and respective symbol appearance on a file
    void write_index(string path)
    {
        ofstream ofs(path);

        if (!ofs.good())
        {
            cout << "Could not locate specified path." << endl;
            exit(2);
        }

        for (pair<string, map<string, int>> entry : this->index)
        {
            ofs << entry.first << "\t";
            for (pair<string, int> symbol : entry.second)
            {
                ofs << symbol.first;
                ofs << "\t";
                ofs << symbol.second;
                ofs << "\t";
            }
            ofs << endl;
        }
        ofs.close();
    }

    void load_index(string path)
    {
        this->reset_fcm();
        ifstream ifs(path);
        string line;

        if (!ifs.good())
        {
            cout << "Could not locate specified path." << endl;
            exit(2);
        }

        while (getline(ifs, line))
        {
            vector<string> spl = split(line, '\t');
            string ctx = spl[0];

            for (auto i = ctx.cbegin(); i != ctx.cend(); i++)
            {
                string s = "";
                s += *i;
                this->alphabet.insert(s);
            }
            this->count_ctx.emplace(ctx, 0);

            map<string, int> tmp;
            for (int i = 1; i < spl.size(); i = i + 2)
            {
                tmp.emplace(spl[i], stoi(spl[i + 1]));
                this->count_ctx[ctx] += stoi(spl[i + 1]);
                this->alphabet.insert(spl[i]);
            }
            this->index.emplace(ctx, tmp);
            /* size_t pos = line.find("\t");
            string ctx = line.substr(0, pos);
            this->k = ctx.length();
            line.erase(line.cbegin(), line.cbegin() + pos + 1);

            for (auto i = ctx.cbegin(); i != ctx.cend(); i++)
            {
                string s = "";
                s += *i;
                this->alphabet.insert(s);
            }

            map<string, int> tmp;

            this->count_ctx.emplace(ctx, 0);

            while (line.find("\t") != string::npos)
            {
                pos = line.find("\t");
                string symbol = line.substr(0, pos);
                line.erase(line.cbegin(), line.cbegin() + pos + 1);
                pos = line.find("\t");
                int count = stoi(line.substr(0, pos));
                line.erase(line.cbegin(), line.cbegin() + pos + 1);
                tmp.emplace(symbol, count);

                this->count_ctx[ctx] += count;
                this->alphabet.insert(symbol);
            }
            this->index.emplace(ctx, tmp); */
        }
        ifs.close();
    }

    // prints the information on the index dictionary
    void show_index()
    {
        for (auto i = this->index.cbegin(); i != this->index.cend(); i++)
        {
            cout << endl
                 << "Context = " << i->first << endl;

            for (auto j = i->second.cbegin(); j != i->second.cend(); j++)
            {
                if (j->first == "\n")
                    cout << "\\n - " << j->second << endl;
                else if (j->first == " ")
                    cout << "\\w - " << j->second << endl;
                else
                    cout << j->first << " - " << j->second << endl;
            }
        }
    }

    // counts the number of ocurrences of symbol after a specific context
    void add_to_index(string context, string symbol)
    {
        if (this->index.count(context) == 0)
        {
            map<string, int> tmp;
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

    // returns context if exists
    string get_context(string ctx)
    {
        if (this->index.count(ctx) == 0)
        {
            return "";
        }
        return ctx;
    }

    // returns alphabet size
    int get_alphabet_size()
    {
        return this->alphabet.size();
    }

    // returns alphabet size
    int get_ctx_num_occurrunces(string ctx)
    {
        return this->count_ctx[ctx];
    }

    // TODO: new, need to verify this
    int get_index(string ctx, string c)
    {
        return this->index[ctx].count(c);
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

    // calculates the apparition of a specific symbol for a specific context
    float get_symbol_probability(string ctx, string symbol)
    {
        float res = ((float)this->index[ctx][symbol] + (float)this->alpha) / ((float)this->count_ctx[ctx] + ((float)this->alpha * (float)this->alphabet.size()));
        // cout << "symbol_prob = " << res << endl;
        return res;
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
        map<string, int> tmp = this->index[ctx];
        float entropy = 0;
        for (auto i = tmp.cbegin(); i != tmp.cend(); i++)
        {
            // cout << "symbol = " << i->first << endl;
            float prob = (float)this->get_symbol_probability(ctx, i->first);
            entropy += (prob * -log2(prob));
        }
        // cout << "context_entropy = " << -entropy << endl;
        return entropy;
    }

    // calculates the probability of a specific context of the model
    float get_context_probability(string ctx)
    {
        float res = (float)this->count_ctx[ctx] / (float)this->get_total_ocurrences();
        // cout << "context_prob = " << res << endl;
        return res;
    }

    void show_ctx_ocurrences()
    {
        for (auto i = this->count_ctx.cbegin(); i != this->count_ctx.cend(); i++)
        {
            cout << i->first << " - " << i->second << endl;
        }
    }

    void show_alphabet()
    {
        for (auto i = this->alphabet.cbegin(); i != this->alphabet.cend(); i++)
        {
            if (*i == " ")
                cout << "\\w" << endl;
            else
                cout << *i << endl;
        }
    }

    vector<string> split(string s, char c)
    {
        vector<string> spl;
        string tmp = "";
        for (int i = 0; i < s.length(); i++)
        {
            if (s[i] == c)
            {
                spl.push_back(tmp);
                tmp = "";
                i++;
            }
            tmp += s[i];
        }
        // spl.push_back(tmp);
        return spl;
    }

    void reset_fcm()
    {
        this->index.erase(this->index.cbegin(), this->index.cend());
        this->count_ctx.erase(this->count_ctx.cbegin(), this->count_ctx.cend());
        this->alphabet.erase(this->alphabet.cbegin(), this->alphabet.cend());
        this->num_chars = 0;
        this->text.erase(this->text.cbegin(), this->text.cend());
    }
};