#include <iostream>
#include <stdio.h>
#include <sndfile.hh>
using namespace std;

int main()
{
    SF_INFO *info = new SF_INFO();
    info->format = SF_FORMAT_WAV;
    SNDFILE *in = sf_open("audio.wav", SFM_READ, info);
    SNDFILE *out = sf_open("audio_out.wav", SFM_WRITE, info);

    short *sample = (short *)malloc(sizeof(short *));

    int a = sf_read_short(in, sample, 10);

    cout << a << endl;

    sf_close(in);
    sf_close(out);

    return 0;
}
