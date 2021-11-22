#include <iostream>
#include <fstream>
#include <map>
using namespace std;

int main(){
    ifstream ifs("simpleText.txt");

    map<char, int> m;
    
    while(!ifs.eof()){
        string s;
        ifs >> s;
        for(int i=0;i<s.length();i++){
            if(m.count(s[i]) == 0) m[s[i]] = 1;
            else m[s[i]]++;
        }
    }

    for(pair<char,int> i : m){
        cout << i.first << " - " << i.second << endl;
    }

    return 0;
}