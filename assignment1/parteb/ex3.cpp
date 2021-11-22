#include <iostream>
#include "../audio/AudioFile.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "USAGE: ./program <input_audio> <output_audio>" << endl;
        return -1;
    }

    AudioFile<double> audioIn, audioOut;

    bool loaded = audioIn.load(argv[1]);

    /* Check if input is a valid audio file */
    assert(loaded);

    audioOut.setNumChannels(audioIn.getNumChannels());
    audioOut.setNumSamplesPerChannel(audioIn.getNumSamplesPerChannel());

    for (int i = 0; i < audioIn.getNumSamplesPerChannel(); i++)
    {
        for (int channel = 0; channel < audioIn.getNumChannels(); channel++)
        {
            audioOut.samples[channel][i] = audioIn.samples[channel][i];
        }
    }

    audioOut.save(argv[2], AudioFileFormat::Wave);

    return 0;
}
