#include "fcm.hpp"

class lang
{
private:
    float estimated_n_bits = 0;

public:
    lang(string reference, string target, int k, float alpha)
    {
        string ctx = "";
        vector<string> make_ctx(k, "");
        char p;
        int count_symbols = 0;

        // create a model of the reference text
        FCM ref_text(k, alpha);

        // read reference text file
        // ref_text.read_file(reference);

        // TODO: Check if the reference text already exists in cache. If so, use it. If not, added to it.

        // load reference text from cache
        ref_text.load_index(reference);

        // open target text file
        ifstream ifs(target);
        if (!ifs.good())
        {
            cout << "File \"" << target << "\" could not be located." << endl;
            exit(1);
        }

        // convert the percentage of alpha, passed as argument, to decimal ]0,1]
        alpha = (float)alpha / 100.0;

        // iterate over all characters in the target text
        while (ifs.get(p))
        {
            string c = "";
            c += p;

            switch ((int)p)
            {
            case -17:
                p = ifs.get();
                c += p;
                p = ifs.get();
                c += p;
                break;
            case -30:
                p = ifs.get();
                c += p;
                p = ifs.get();
                c += p;
                break;
            default:
                if ((int)p > -1)
                    break;
                p = ifs.get();
                c += p;
                break;
            }

            if (ref_text.get_context(ctx) == ctx)
            {
                // check if the symbol exists after the context in the reference text
                if (ref_text.get_index(ctx, c) != 0)
                {
                    estimated_n_bits += (float)(-log2(ref_text.get_symbol_probability(ctx, c)));
                }
                else
                {
                    estimated_n_bits += (float)(-log2(
                        alpha / ((float)ref_text.get_ctx_num_occurrunces(ctx) + alpha * (float)ref_text.get_alphabet_size())));
                }
            }
            else
            {
                estimated_n_bits += (float)(-log2(1.0 / (float)ref_text.get_alphabet_size()));
            }

            make_ctx.push_back(c);
            if (make_ctx.size() > k)
            {
                make_ctx.erase(make_ctx.begin(), make_ctx.begin() + 1);
            }

            ctx = "";
            for (int i = 0; i < k; i++)
            {
                ctx += make_ctx[i];
            }

            count_symbols++;
        }

        // calculate the estimated number of bits per symbol
        estimated_n_bits = estimated_n_bits / count_symbols;

        // close target text file
        ifs.close();

        ref_text.reset_fcm();

        cout << "Estimated number of bits per symbol: " << estimated_n_bits << endl;
    }

    float get_estimated_bits()
    {
        return this->estimated_n_bits;
    }
};