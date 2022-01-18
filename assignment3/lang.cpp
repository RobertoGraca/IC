#include "fcm.hpp"

int main(int argc, char **argv)
{
    if (argc != 5)
    {
        cout << "USAGE: ./lang <reference_text> <target_text> <order_model> <smoothing_parameter>" << endl;
        exit(1);
    }

    int estimated_n_bits = 0;
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
        /* estimated_n_bits += - log2(probability char); */
        estimated_n_bits += 1;
    }

    // close target text file
    ifs.close();

    cout << "" << estimated_n_bits << "" << endl;

    return estimated_n_bits;
}