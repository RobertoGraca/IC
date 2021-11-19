#include <iostream>
#include <cmath>
#include "../audio/AudioFile.h"
using namespace std;

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        cout << "USAGE: ./program <input_audio> <output_audio>" << endl;
        return -1;
    }

    AudioFile<double> input, input2;
    bool loaded1 = input.load(argv[1]);
    bool loaded2 = input2.load(argv[2]);
    assert(loaded1);
    assert(loaded2);

    float mse = 0;
    int numChannels = input.getNumChannels(), numSamples = input.getNumSamplesPerChannel();

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