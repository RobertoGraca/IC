#include <iostream>
#include <fstream>
#include "audio/AudioFile.h"
#include "math.h"
#include "assert.h"
#include <map>
using namespace std;
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
    double *residual = (double *)malloc(arr_size * sizeof(double));
    for (int i = 0; i < arr_size; i++)
    {
        residual[i] = sample[i] - pred_array[i];
    }
    return residual;
}

int main(int argc, char *argv[])
{
    assertm(argc == 3, "Use ./ex2 <path to the input audio file> <path to the output histogram file> ");
    map<double, int> mChannel0;
    map<double, int> mResidual0;
    map<double, int> mChannel1;
    map<double, int> mResidual1;
    AudioFile<double> audio;
    double prob;
    double entropyResidualCh0, entropyResidualCh1, entropyOriginalCh0, entropyOriginalCh1, entropyResiduaTotal, entropyOriginalTotal;
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
    //cout << num_channels << endl;
    for (int nSamples = 0; nSamples < audio.getNumSamplesPerChannel(); nSamples++)
    {
        double currenteSampleChannel0 = audio.samples[0][nSamples];
        samplesChannel0[nSamples] = currenteSampleChannel0;
        if (mChannel0.count(currenteSampleChannel0) == 0)
        {
            mChannel0[currenteSampleChannel0] = 1;
        }
        else
        {
            mChannel0[currenteSampleChannel0]++;
        }
        if (num_channels == 2)
        {
            double currenteSampleChannel1 = audio.samples[1][nSamples];
            samplesChannel1[nSamples] = currenteSampleChannel1;
            if (mChannel1.count(currenteSampleChannel1) == 0)
            {
                mChannel1[currenteSampleChannel1] = 1;
            }
            else
            {
                mChannel1[currenteSampleChannel1]++;
            }
        }
    }
    pred_arrayChannel0 = predictor(arr_size, samplesChannel0);
    resiChannel0 = residual(arr_size, samplesChannel0, arr_size, pred_arrayChannel0);
    if (num_channels == 2)
    {
        pred_arrayChannel1 = predictor(arr_size, samplesChannel1);
        resiChannel1 = residual(arr_size, samplesChannel1, arr_size, pred_arrayChannel1);
    }

    for (int i = 0; i < arr_size; i++)
    {

        if (mResidual0.count(resiChannel0[i]) == 0)
        {
            mResidual0[resiChannel0[i]] = 1;
        }
        else
        {
            mResidual0[resiChannel0[i]]++;
        }
        if (num_channels == 2)
        {
            if (mResidual1.count(resiChannel0[i]) == 0)
            {
                mResidual1[resiChannel1[i]] = 1;
            }
            else
            {
                mResidual1[resiChannel1[i]]++;
            }
        }
    }
    ofs << "Channel 0" << endl;
    for (auto x : mResidual0)
    {
        ofs << x.first << " => " << x.second << endl;
    }
    for (auto x : mResidual0)
    {
        prob = (double)(x.second) / arr_size;
        entropyResidualCh0 += prob * log(prob);
    }

    for (auto x : mChannel0)
    {
        prob = (double)(x.second) / arr_size;
        entropyOriginalCh0 += prob * log(prob);
    }
    if (num_channels == 2)
    {
        ofs << "____________________________________________________________" << endl;
        ofs << "Channel 1" << endl;
        for (auto x : mResidual1)
        {
            ofs << x.first << " => " << x.second << endl;
        }
        ofs.close();
        for (auto x : mResidual1)
        {
            prob = (double)(x.second) / arr_size;
            entropyResidualCh1 += prob * log(prob);
        }
        for (auto x : mChannel1)
        {
            prob = (double)(x.second) / arr_size;
            entropyOriginalCh1 += prob * log(prob);
        }
        entropyOriginalTotal = -((entropyOriginalCh0 + entropyOriginalCh1) / 2);
        entropyResiduaTotal = -((entropyResidualCh0 + entropyResidualCh1) / 2);
    }
    else
    {
        entropyOriginalTotal = -entropyOriginalCh0;
        entropyResiduaTotal = -entropyResidualCh0;
    }

    cout << "Entropy Original Data: " << entropyOriginalTotal << endl;
    cout << "Entropy Residuals After Predictor : " << entropyResiduaTotal << endl;
}
