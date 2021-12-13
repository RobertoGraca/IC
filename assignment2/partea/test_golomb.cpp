#include <iostream>
#include "Golomb.h"
#include <vector>

using namespace std;

int main()
{
    vector<bool> enc;
    cout << "Golomb m = 4" << endl;
    Golomb g4(4);

    enc = g4.encode(11);
    for (auto i = enc.begin(); i != enc.end(); i++)
    {
        cout << *i << " ";
    }
    cout << endl;
    cout << g4.decode(enc) << endl;
    enc.erase(enc.cbegin(), enc.cend());

    cout << "\nGolomb m = 7" << endl;
    Golomb g7(7);

    enc = g7.encode(95);
    for (auto i = enc.begin(); i != enc.end(); i++)
    {
        cout << *i << " ";
    }
    cout << endl;
    cout << g7.decode(enc) << endl;
    enc.erase(enc.cbegin(), enc.cend());

    cout << "\nGolomb m = 9" << endl;
    Golomb g9(9);

    enc = g9.encode(89);
    for (auto i = enc.begin(); i != enc.end(); i++)
    {
        cout << *i << " ";
    }
    cout << endl;
    cout << g9.decode(enc) << endl;
    enc.erase(enc.cbegin(), enc.cend());

    cout << "\nGolomb m = 21" << endl;
    Golomb g21(21);

    enc = g21.encode(89);
    for (auto i = enc.begin(); i != enc.end(); i++)
    {
        cout << *i << " ";
    }
    cout << endl;
    cout << g21.decode(enc) << endl;
    enc.erase(enc.cbegin(), enc.cend());

    return 0;
}