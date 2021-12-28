#include <iostream>
#include <fstream>
#include "audio/AudioFile.h"
#include "../a/Golomb.h"
#include "assert.h"
#include <list>
#include <string.h>
#include <map>
#include <chrono>
using namespace std;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

#define assertm(exp, msg) assert(((void)msg, exp))

double *predictor(int arr_size, double *sample)
{
    double *pred_array = (double *)malloc(arr_size * sizeof(double));
    for (int i = 0; i < arr_size; i++)
    {
        if (i == 0)
        {
            pred_array[i] = 0.0;
        }
        else if (i == 1)
        {
            pred_array[i] = sample[i - 1];
        }
        else if (i == 2)
        {
            pred_array[i] = (2 * sample[i - 1]) - sample[i - 2];
        }
        else
        {
            pred_array[i] = (3 * sample[i - 1]) - (3 * sample[i] - 2) + sample[i - 3];
        }
    }
    return pred_array;
}

double *residual(int arr_size, double *sample, int arr2_size, double *pred_array)
{
    assert(arr_size == arr2_size);
    map<char, int> m;
    double *residual = (double *)malloc(arr_size * sizeof(double));
    for (int i = 0; i < arr_size; i++)
    {
        residual[i] = sample[i] - pred_array[i];
    }
    return residual;
}

int main(int argc, char *argv[])
{
    vector<int> encIntCh0;
    vector<int> encDecCh0;
    assertm(argc > 1, "Use ./ex1 <path to the input audio file> <path to the output encoded file> ");
    AudioFile<double> audio;
    AudioFile<double> audioOut;
    bool loaded = audio.load(argv[1]);
    ofstream ofs(argv[2]);
    assert(loaded);
    int arr_size = audio.getNumSamplesPerChannel();
    int num_channels = audio.getNumChannels();
    double *samplesChannel0 = (double *)malloc(arr_size * sizeof(double));
    double *samplesChannel1 = (double *)malloc(arr_size * sizeof(double));
    double *pred_arrayChannel0 = (double *)malloc(arr_size * sizeof(double));
    double *resiChannel0 = (double *)malloc(arr_size * sizeof(double));
    double *pred_arrayChannel1 = (double *)malloc(arr_size * sizeof(double));
    double *resiChannel1 = (double *)malloc(arr_size * sizeof(double));

    for (int nSamples = 0; nSamples < audio.getNumSamplesPerChannel(); nSamples++)
    {
        double currentSampleChannel0 = audio.samples[0][nSamples];
        samplesChannel0[nSamples] = currentSampleChannel0;

        if (num_channels == 2)
        {
            double currentSampleChannel1 = audio.samples[1][nSamples];
            samplesChannel1[nSamples] = currentSampleChannel1;
        }
    }
    pred_arrayChannel0 = predictor(arr_size, samplesChannel0);
    resiChannel0 = residual(arr_size, samplesChannel0, arr_size, pred_arrayChannel0);
    for (int i = 0; i < arr_size; i++)
    {
        int b = pred_arrayChannel0[i];
        double a = b - pred_arrayChannel0[i];
        string s = to_string(a);
        int size = s.length() - 2;
        encIntCh0.push_back(b);
        encDecCh0.push_back(a * pow(10, size));
    }

    /* for(auto x: encIntCh0){
        cout<<x<<endl;
    } */
    /* for(auto x: encDecCh0){
        cout<<x<<endl;
    }  */
    vector<int> dec;
    Golomb gIntCh0(128, "gInt.bits");
    cout << "Channel0" << endl;
    auto t1 = high_resolution_clock::now();
    gIntCh0.encode(encIntCh0);
    auto t2 = high_resolution_clock::now();
    auto ms_int = duration_cast<milliseconds>(t2 - t1);

    /* for (auto i = encIntCh0.cbegin(); i != encIntCh0.cend(); i++)
    {
        cout << *i << " ";
    }
    cout << endl; */
    cout << ms_int.count() << "ms to encode" << endl;

    Golomb gDecCh0(4096, "gDec.bits");
    //cout << "ECONDER DEC" << endl;
    t1 = high_resolution_clock::now();
    gDecCh0.encode(encDecCh0);
    t2 = high_resolution_clock::now();
    ms_int = duration_cast<milliseconds>(t2 - t1);
   /*  for (auto i = encDecCh0.cbegin(); i != encDecCh0.cend(); i++)
    {
        cout << *i << " ";
    }
    cout << endl; */
    cout << ms_int.count() << "ms to encode" << endl;
    t1 = high_resolution_clock::now();
    dec = gIntCh0.decode();
    t2 = high_resolution_clock::now();
    ms_int = duration_cast<milliseconds>(t2 - t1);
    cout << "Decoded the values: ";
    /* for (auto i = dec.cbegin(); i != dec.cend(); i++)
    {
        cout << *i << " ";
    }
    cout << endl; */
    dec.erase(dec.cbegin(), dec.cend());
    cout << "Encoded file used " << gIntCh0.get_filename_num_chars() * 8 << " bits and took " << ms_int.count() << "ms to decode" << endl;

    gIntCh0.delete_bin_file();

    t1 = high_resolution_clock::now();
    dec = gDecCh0.decode();
    t2 = high_resolution_clock::now();
    ms_int = duration_cast<milliseconds>(t2 - t1);
    cout << "Decoded the values: ";
    /* for (auto i = dec.cbegin(); i != dec.cend(); i++)
    {
        cout << *i << " ";
    }
    cout << endl; */
    dec.erase(dec.cbegin(), dec.cend());
    cout << "Encoded file used " << gDecCh0.get_filename_num_chars() * 8 << " bits and took " << ms_int.count() << "ms to decode" << endl;
    gDecCh0.delete_bin_file();

    if (num_channels == 2)
    {
        vector<int> encIntCh1;
        vector<int> encDecCh1;
        pred_arrayChannel1 = predictor(arr_size, samplesChannel1);
        resiChannel1 = residual(arr_size, samplesChannel1, arr_size, pred_arrayChannel1);
        for (int i = 0; i < arr_size; i++)
        {
            int b = pred_arrayChannel1[i];
            double a = b - pred_arrayChannel1[i];
            string s = to_string(a);
            int size = s.length() - 2;
            encIntCh1.push_back(b);
            encDecCh1.push_back(a * pow(10, size));
        }

        /* for(auto x: encIntCh1){
            cout<<x<<endl;
        } */
        /* for(auto x: encDecCh1){
            cout<<x<<endl;
        }  */
     
        Golomb gIntCh1(128, "gInt.bits");
        cout << "Channel1" << endl;
        auto t1 = high_resolution_clock::now();
        gIntCh1.encode(encIntCh1);
        auto t2 = high_resolution_clock::now();
        auto ms_int = duration_cast<milliseconds>(t2 - t1);

        /* for (auto i = encIntCh1.cbegin(); i != encIntCh1.cend(); i++)
        {
            cout << *i << " ";
        }
        cout << endl; */

        cout << ms_int.count() << "ms to encode" << endl;

        Golomb gDecCh1(4096, "gDec.bits");
        //cout << "ECONDER DEC" << endl;
        t1 = high_resolution_clock::now();
        gDecCh1.encode(encDecCh1);
        t2 = high_resolution_clock::now();
        ms_int = duration_cast<milliseconds>(t2 - t1);
       /*  for (auto i = encDecCh1.cbegin(); i != encDecCh1.cend(); i++)
        {
            cout << *i << " ";
        }
        cout << endl; */
        cout << ms_int.count() << "ms to encode" << endl;
        t1 = high_resolution_clock::now();
        dec = gIntCh1.decode();
        t2 = high_resolution_clock::now();
        ms_int = duration_cast<milliseconds>(t2 - t1);
        cout << "Decoded the values: ";
        /* for (auto i = dec.cbegin(); i != dec.cend(); i++)
        {
            cout << *i << " ";
        }
        cout << endl; */
        dec.erase(dec.cbegin(), dec.cend());
        cout << "Encoded file used " << gIntCh1.get_filename_num_chars() * 8 << " bits and took " << ms_int.count() << "ms to decode" << endl;

        gIntCh1.delete_bin_file();

        t1 = high_resolution_clock::now();
        dec = gDecCh1.decode();
        t2 = high_resolution_clock::now();
        ms_int = duration_cast<milliseconds>(t2 - t1);
        cout << "Decoded the values: ";
        /* for (auto i = dec.cbegin(); i != dec.cend(); i++)
        {
            cout << *i << " ";
        }
        cout << endl; */
        dec.erase(dec.cbegin(), dec.cend());
        cout << "Encoded file used " << gDecCh1.get_filename_num_chars() * 8 << " bits and took " << ms_int.count() << "ms to decode" << endl;
        gDecCh1.delete_bin_file();
    }

}
