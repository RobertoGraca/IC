#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char** argv){

    if(argc != 3){
        cout << "USAGE: ./programName file1 file2" << endl;
    }else{
        ifstream ifs(argv[1]);
        ofstream ofs(argv[2]);

        while(!ifs.eof()){
            string s;
            ifs >> s;
            for(int i=0;i<s.length();i++){
                ofs << s[i];
            }
            ofs << endl;
        }
    }
    return 0;
}