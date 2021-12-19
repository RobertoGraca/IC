#include <iostream>
#include "Golomb.h"
#include <vector>

using namespace std;

int main()
{
    string to_encode("Write here stuff to encode! :)");
    vector<int> enc;
    for (int i = 0; i < to_encode.length(); i++)
    {
        enc.push_back(to_encode.at(i));
    }
    vector<int> dec;

    cout << "Golomb m = 4" << endl;
    Golomb g4(4);
    g4.encode(enc);
    cout << "Encoded the values: ";
    for (auto i = enc.cbegin(); i != enc.cend(); i++)
    {
        cout << (char)*i << " ";
    }
    cout << endl;
    dec = g4.decode();
    cout << "Decoded the values: ";
    for (auto i = dec.cbegin(); i != dec.cend(); i++)
    {
        cout << (char)*i << " ";
    }
    cout << endl;
    dec.erase(dec.cbegin(), dec.cend());
    cout << "Encoded file used " << g4.get_filename_num_chars() * 8 << " bits" << endl;

    cout << "\nGolomb m = 7" << endl;
    Golomb g7(7);
    g7.encode(enc);
    cout << "Encoded the values: ";
    for (auto i = enc.cbegin(); i != enc.cend(); i++)
    {
        cout << (char)*i << " ";
    }
    cout << endl;
    dec = g7.decode();
    cout << "Decoded the values: ";
    for (auto i = dec.cbegin(); i != dec.cend(); i++)
    {
        cout << (char)*i << " ";
    }
    cout << endl;
    dec.erase(dec.cbegin(), dec.cend());
    cout << "Encoded file used " << g7.get_filename_num_chars() * 8 << " bits" << endl;

    cout << "\nGolomb m = 9" << endl;
    Golomb g9(9);
    g9.encode(enc);
    cout << "Encoded the values: ";
    for (auto i = enc.cbegin(); i != enc.cend(); i++)
    {
        cout << (char)*i << " ";
    }
    cout << endl;
    dec = g9.decode();
    cout << "Decoded the values: ";
    for (auto i = dec.cbegin(); i != dec.cend(); i++)
    {
        cout << (char)*i << " ";
    }
    cout << endl;
    dec.erase(dec.cbegin(), dec.cend());
    cout << "Encoded file used " << g9.get_filename_num_chars() * 8 << " bits" << endl;

    cout << "\nGolomb m = 21" << endl;
    Golomb g21(21);
    g21.encode(enc);
    cout << "Encoded the values: ";
    for (auto i = enc.cbegin(); i != enc.cend(); i++)
    {
        cout << (char)*i << " ";
    }
    cout << endl;
    dec = g21.decode();
    cout << "Decoded the values: ";
    for (auto i = dec.cbegin(); i != dec.cend(); i++)
    {
        cout << (char)*i << " ";
    }
    cout << endl;
    dec.erase(dec.cbegin(), dec.cend());
    cout << "Encoded file used " << g21.get_filename_num_chars() * 8 << " bits" << endl;

    return 0;
}