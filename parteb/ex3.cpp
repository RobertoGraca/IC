#include <iostream>
#include "../audio/AudioFile.h"

using namespace std;

int main(int argc, char *argv[])
{
    AudioFile<double> audioIn, audioOut;

    bool loaded = audioIn.load(argv[1]);

    audioOut.setNumChannels(audioIn.getNumChannels());
    audioOut.setNumSamplesPerChannel(audioIn.getNumSamplesPerChannel());

    /* If you hit this assert, then the file path above probably doesn't refer to a valid audio file */
    assert(loaded);

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
