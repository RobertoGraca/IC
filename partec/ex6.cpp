#include <iostream>
#include <fstream>
#include <map>
#include "AudioFile.h"
#include "math.h"
using namespace std;

int main(int argc, char *argv[])
{
    AudioFile<double> audio;
    bool loaded = audio.load(argv[1]);
    map<double, int> m0;
    map<double, int> m1;
    ofstream ofs0("ex6channel0.txt");
    ofstream ofs1("ex6channel1.txt");
    int totalSamples0 = 0;
    int totalSamples1 = 0;
    double entropy0 = 0;
    double entropy1 = 0;
    double prob = 0;

    /* If you hit this assert, then the file path above probably doesn't refer to a valid audio file */
    assert(loaded);

    for (int nSamples = 0; nSamples < audio.getNumSamplesPerChannel(); nSamples++)
    {
        if (m0.count(audio.samples[0][nSamples]) == 0)
        {
            m0[audio.samples[0][nSamples]] = 1;
        }
        else
        {
            m0[audio.samples[0][nSamples]]++;
        }
        totalSamples0++;
    }

    for (int nSamples = 0; nSamples < audio.getNumSamplesPerChannel(); nSamples++)
    {
        if (m1.count(audio.samples[1][nSamples]) == 0)
        {
            m1[audio.samples[1][nSamples]] = 1;
        }
        else
        {
            m1[audio.samples[1][nSamples]]++;
        }
        totalSamples1++;
    }

    /* audio2.setNumChannels(1);                                                     
    for (int nSamples = 0; nSamples < audio2.getNumSamplesPerChannel(); nSamples++)
    {
        if (m2.count(audio2.samples[1][nSamples]) == 0)
        {
            m2[audio2.samples[0][nSamples]] = 1;
            cout<<"if1"<<endl;                                                         //it doesn't work
        }
        else
        {
            m2[audio2.samples[0][nSamples]]++;
            cout<<"if2"<<endl;
        }
        totalSamples2++;
        
    } */

    for (auto x : m0)
    {
        prob = (double)(x.second) / totalSamples0;
        entropy0 += prob*log(prob);
    }

    prob = 0;

    for (auto x : m1)
    {
        prob = (double)(x.second) / totalSamples1;
        entropy1 += prob*log(prob);
    }
    
    prob = 0;

    for (auto x : m0)
    {
        ofs0 << x.first << " => " << x.second << endl;
    }

    for (auto x : m1)
    {
        ofs1 << x.first << " => " << x.second << endl;
    }

    /* for (auto x : m2)
    {
        ofs2 << x.first << " => " << x.second << endl;
    } */

    entropy0 = -entropy0;
    cout<<"Entropy of channel 0: "<<entropy0<<endl;
    entropy1 = -entropy1;
    cout<<"Entropy of channel 1: "<<entropy1<<endl;
    ofs0.close(); 
    ofs1.close(); 
    return 0;           //to see the histogram run ex6.py and open ex6channel0.png and ex6channel1.png
}
