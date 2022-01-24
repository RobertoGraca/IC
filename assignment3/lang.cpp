#include "fcm.hpp"

int main(int argc, char **argv)
{
    if (argc != 5)
    {
        cout << "USAGE: ./lang <reference_text> <target_text> <order_model> <smoothing_parameter>" << endl;
        exit(1);
    }

    float estimated_n_bits = 0;
    string ctx = "";
    char c;

    // create a model of the reference text
    FCM ref_text(atoi(argv[3]), atoi(argv[4]));

    // read reference text file
    ref_text.read_file(argv[1]);

    // open target text file
    ifstream ifs(argv[2]);
    if (!ifs.good())
    {
        cout << "File \"" << argv[2] << "\" could not be located." << endl;
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
                                        atoi(argv[4]) 
                                            / (ref_text.get_ctx_num_occurrunces(ctx)
                                                + atoi(argv[4]) * (float)ref_text.get_alphabet_size()
                                            )
                                        )
                                    );
            }
        }
        else
        {
            estimated_n_bits += (float)(-log2(1.0 / (float)ref_text.get_alphabet_size()));
        }
        ctx += c;
        if (ctx.length() > atoi(argv[3]))
        {
            ctx.erase(ctx.begin(), ctx.begin() + 1);
        }
    }

    // close target text file
    ifs.close();

    cout << "" << estimated_n_bits << "" << endl;

    return estimated_n_bits;
}