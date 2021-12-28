#include <iostream>
#include <fstream>
#include "audio/AudioFile.h"
#include "../a/Golomb.h"
#include "assert.h"
#include <list>
#include <string.h>
#include <map>
using namespace std;
#define assertm(exp, msg) assert(((void)msg, exp))

double *predictor(int arr_size, double *sample){
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

double *residual(int arr_size, double *sample, int arr2_size, double *pred_array){
    assert(arr_size == arr2_size);
    map<char, int> m;
    double *residual = (double *)malloc(arr_size * sizeof(double));
    for (int i = 0; i < arr_size; i++)
    {
        residual[i] = sample[i] - pred_array[i];
    }
    return residual;
}

double *quantitization(int arr_size, double* sample){
    double *quant = (double *)malloc(arr_size * sizeof(double));
    for (int i = 0; i < arr_size; i++)
    {
        int samples = sample[i] * 32768;
        quant[i] = (samples >> 8) << 8;
    }
    return quant;
}


int main(int argc, char *argv[]){
    vector<int> encInt;
    vector<int> encDec;
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
        encInt.push_back(b);
        encDec.push_back(abs(a * pow(10, size)));
    }
    for (auto x : encInt)
    {
        cout << x << endl;
    }
    for (auto x : encDec)
    {
        cout << x << endl;
    }
    Golomb g641(64, "g64.bits");
    g641.encode(encInt);
    /*  Golomb g642(64,"g64.bits");
     g642.encode(encDec);
     cout << "Encoded the values: "; */
    /* for (auto i = encInt.cbegin(); i != encInt.cend(); i++)
    {
        cout << (char)*i << " ";
    }
    cout << endl; */
    if (num_channels == 2)
    {
        pred_arrayChannel1 = predictor(arr_size, samplesChannel1);
        resiChannel1 = residual(arr_size, samplesChannel1, arr_size, pred_arrayChannel1);
    }





}
