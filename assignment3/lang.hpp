#include "fcm.hpp"

class lang
{
private:
    float estimated_n_bits = 0;
    string ctx = "";
    char c;

public:
    lang(string reference, string target, int k, int alpha)
    {

        // create a model of the reference text
        FCM ref_text(k, alpha);

        // read reference text file
        ref_text.read_file(reference);

        // open target text file
        ifstream ifs(target);
        if (!ifs.good())
        {
            cout << "File \"" << target << "\" could not be located." << endl;
            exit(1);
        }

        /* while  PERCORRER TODOS OS CARACTERES E CHAMAR A FUNÇÃO GET_CTX E SOMAR */
        while (ifs.get(c))
        {
            cout << "" << ctx << "" << endl;
            if (ref_text.get_context(ctx) == ctx)
            {
                cout << "breakpoint 1" << endl;
                if (1) // ver se o símbolo existe a seguir ao contexto, no ref_text
                {
                    estimated_n_bits += (float)(-log2(ref_text.get_symbol_probability(ctx, "" + c)));
                }
                else
                {
                    estimated_n_bits += (float)(-log2(
                        alpha / (ref_text.get_ctx_num_occurrunces(ctx) + alpha * (float)ref_text.get_alphabet_size())));
                }
            }
            else
            {
                estimated_n_bits += (float)(-log2(1.0 / (float)ref_text.get_alphabet_size()));
            }
            ctx += c;
            if (ctx.length() > k)
            {
                ctx.erase(ctx.begin(), ctx.begin() + 1);
            }
        }

        // close target text file
        ifs.close();

        cout << "" << estimated_n_bits << "" << endl;
    }

    int estimated_bits()
    {
        return this->estimated_n_bits;
    }
};