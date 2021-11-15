#include <iostream>
#include "audio/AudioFile.h"

using namespace std;

int main(int argc, char* argv[]){
    AudioFile<double> audio;

    bool loaded = audio.load(argv[1]);
         
    /* If you hit this assert, then the file path above probably doesn't refer to a valid audio file */
    assert(loaded);

    for (int i = 0; i < audio.getNumSamplesPerChannel(); i++){
        for (int channel = 0; channel < audio.getNumChannels(); channel++){
            audio.samples[channel][i] = audio.samples[channel][i];
        }
    }

    audio.save(argv[2], AudioFileFormat::Wave);

    return 0;
}