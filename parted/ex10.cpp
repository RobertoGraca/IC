#include <iostream>
#include <cmath>
#include "../audio/AudioFile.h"
using namespace std;

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        cout << "USAGE: ./program <input_audio_1> <input_audio_2>" << endl;
        return -1;
    }

    AudioFile<double> input, input2;
    string path = "../audio/";
    bool loaded1 = input.load(path + argv[1]);

    /* Check if input1 is a valid audio file */
    assert(loaded1);

    bool loaded2 = input2.load(argv[2]);

    /* Check if input2 is a valid audio file */
    assert(loaded2);

    int num11 = input.getNumChannels();
    int num12 = input.getNumSamplesPerChannel();
    int num21 = input2.getNumChannels();
    int num22 = input2.getNumSamplesPerChannel();

    float mse = 0;
    int numChannels, numSamples;

    if (num11 > num21)
    {
        numChannels = input2.getNumChannels();
    }
    else
    {
        numChannels = input.getNumChannels();
    }

    if (num12 > num22)
    {
        numSamples = input2.getNumSamplesPerChannel();
    }
    else
    {
        numSamples = input.getNumSamplesPerChannel();
    }

    for (int c = 0; c < numChannels; c++)
    {
        for (int i = 0; i < numSamples; i++)
        {
            mse += pow(input.samples[c][i] - input2.samples[c][i], 2);
        }
    }

    mse = mse / (input.getNumChannels() * input.getNumSamplesPerChannel());
    float psnr = (20 * log10(255)) - (10 * log10(mse));
    string in = argv[1];
    string in2 = argv[2];
    cout << "The PSNR (Peak Signal-to-Noise Ratio) between " + in + " and " + in2 + " is " + to_string(psnr) + " dB" << endl;
}