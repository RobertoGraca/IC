#include "lang.hpp"
#include <filesystem>

// USAGE:
// $ g++ findlang.cpp -std=c++17
// $ ./a.out lang example.txt 4 50

int main(int argc, char **argv)
{
    if (argc != 5)
    {
        cout << "USAGE: ./findlang <reference_texts_directory> <target_text> <order_model> <smoothing_parameter>" << endl;
        exit(1);
    }

    map<float, string> langs;
    string path;

    // iterates over all files in the folder passed as argument
    for (auto const &entry : filesystem::directory_iterator(argv[1]))
    {
        // gets the path for the current file/language dataset
        path = entry.path();

        cout << "File: " << path << endl;

        // creates the new 'lang' object
        lang temp(path, argv[2], atoi(argv[3]), atoi(argv[4]));

        // calculates and stores the estimated number of bits and the corresponding language on the map
        langs.emplace(temp.estimated_bits(), path.substr(0, path.find_last_of(".")).substr(path.find("/") + 1));
    }

    // prints the estimated number of bits for every language
    cout << "" << endl;
    cout << "Results:" << endl;
    for (const auto &[key, value] : langs)
    {
        cout << value << " -> " << key << endl;
    }
    cout << "" << endl;

    // gets the language with the lowest estimated number of bits
    auto min_key = langs.begin()->second;

    cout << "This text is most likely written in " << min_key << endl;

    return 0;
}