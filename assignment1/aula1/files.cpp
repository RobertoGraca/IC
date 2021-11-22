#include <iostream>
#include <fstream>
using namespace std;

int main(){
    ifstream ifs("nums.txt");
    ofstream ofs("out.txt");

    while(!ifs.eof()){
        int x;
        ifs >> x;
        ofs << x << endl;
    }

    return 0;
}