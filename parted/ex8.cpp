#include <iostream>
#include "../audio/AudioFile.h"
using namespace std;

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        cout << "USAGE: ./program <input_audio> <output_audio>" << endl;
        return -1;
    }

    AudioFile<double> input, output;
    string path = "../audio/";
    input.load(path + argv[1]);

    output.setNumChannels(input.getNumChannels());
    output.setNumSamplesPerChannel(input.getNumSamplesPerChannel());

    for (int c = 0; c < input.getNumChannels(); c++)
    {
        for (int i = 0; i < input.getNumSamplesPerChannel(); i++)
        {
            int sample = input.samples[c][i] * 32768;
            output.samples[c][i] = (sample >> 8) << 8;
        }
    }
    string name = argv[1];
    name.erase(remove(name.begin(), name.end(), '.'), name.end());
    output.save(name + to_string('_') + argv[2], AudioFileFormat::Wave);
}