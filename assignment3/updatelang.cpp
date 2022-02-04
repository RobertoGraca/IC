#include <iostream>
#include "fcm.hpp"
// updates existing caches with new contents
int main(int argc, char **argv)
{
    if (argc != 3)
    {
        cout << "USAGE: ./a.out <language_to_update> <language_new_file>" << endl;
        exit(1);
    }
    string lang = argv[1];
    string path = argv[2];

    cout << "Updating " << lang << endl;

    for (int i = 1; i <= 5; i++)
    {
        cout << endl
             << "Computing index for context size " << i << endl;
        FCM fcm(i, 50);
        fcm.load_index("cache/k" + to_string(i) + "/" + lang);
        fcm.read_file(path);
        fcm.write_index("cache/k" + to_string(i) + "/" + lang);
    }

    cout << endl
         << "Updated " << lang << " in the cache." << endl;
    return 0;
}