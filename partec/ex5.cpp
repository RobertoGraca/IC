#include <iostream>
#include <fstream>
#include <map>
#include <opencv2/plot.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int main()
{
    ifstream ifs("lusiadas.txt");
    ofstream ofs("ex5out.txt");

    map<char, int> m;
    int letterCounter = 0;
    double entropy = 0;
    double prob = 0;
    while (!ifs.eof())
    {
        string s;
        ifs >> s;
        for (int i = 0; i < s.length(); i++)
        {
            if (isalpha(s[i]))
            {
                letterCounter++;
                s[i] = tolower(s[i]);
                if (m.count(s[i]) == 0)
                {
                    m[s[i]] = 1;
                }
                else
                {
                    m[s[i]]++;
                }
            }
            else
            {
                continue;
            }
        }
    }

    ifs.close();

    for (auto x : m)
    {
        prob = (double)(x.second) / letterCounter;
        entropy += prob*log(prob);

    }

    for (auto x : m)
    {
        ofs << x.first << " => " << x.second << "\n";
    }
    entropy = -entropy;
    cout<<"Entropy: "<<entropy<<endl;
    ofs.close();                        //to see the histogram run ex5.py and open ex5out.png
}