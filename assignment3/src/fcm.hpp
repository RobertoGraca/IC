#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <fstream>
using namespace std;

class FCM
{
private:
    int k;                             // model order (context size)
    int alpha;                         // smoothing parameter
    map<string, map<char, int>> index; // saves the number of symbol ocurrences for a context
    set<char> alphabet;

public:
    FCM(int k, int alpha)
    {
        this->k = k;
        this->alpha = alpha;
    }

    void read_file(string path)
    {
        ifstream ifs(path);
        stringstream buffer;
        buffer << ifs.rdbuf();
        cout << buffer.str() << endl;
        }

    void fill_map()
    {
    }

    void add_to_index(string context, char symbol)
    {
    }
};