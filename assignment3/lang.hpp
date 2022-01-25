#include "fcm.hpp"

class lang
{
private:
    float estimated_n_bits = 0;
    

public:
    lang(string reference, string target, int k, int alpha)
    {
        string ctx = "";
        char c;

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
            //cout << "---------- Context: " << ctx << " ----------" << endl;
            if (ref_text.get_context(ctx) == ctx)
            {
                //cout << "---------- BREAKPOINT 1 ----------" << endl;
                if (1) // ver se o símbolo existe a seguir ao contexto, no ref_text
                {
                    //cout << "---------- BREAKPOINT 2 ----------" << endl;
                    estimated_n_bits += (float)(-log2(ref_text.get_symbol_probability(ctx, "" + c)));
                }
                else
                {
                    //cout << "---------- BREAKPOINT 3 ----------" << endl;
                    estimated_n_bits += (float)(-log2(
                        alpha / (ref_text.get_ctx_num_occurrunces(ctx) + alpha * (float)ref_text.get_alphabet_size())));
                }
            }
            else
            {
                //cout << "---------- BREAKPOINT 4 ----------" << endl;
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

        cout << "Estimated number of bits: " << estimated_n_bits << endl;
    }

    float estimated_bits()
    {
        // TODO: Check if it is necessary to round the estimated number of bits to the closest int
        return this->estimated_n_bits;
    }
};