#include "fcm.hpp"
#include <iostream>

// g++ newlang.cpp -o newlang
// ./newlang Russian.utf8        -- assumes that the file to be read is already in the "langs" directory and is named accordingly

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cout << "USAGE: ./add_new_lang <new_language_file>" << endl;
        exit(1);
    }
    string path = "langs/";
    path += argv[1];

    cout << "Creating entry for language: " << argv[1] << endl;

    for (int i = 1; i <= 5; i++)
    {
        cout << endl
             << "Computing index for context size " << i << endl;
        FCM fcm(i, 50);
        fcm.read_file(path);
        fcm.write_index("cache/k" + to_string(i) + "/" + argv[1]);
    }

    cout << endl
         << "Added " << argv[1] << " to the system." << endl;

    return 0;
}