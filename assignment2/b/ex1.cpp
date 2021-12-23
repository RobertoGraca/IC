#include <iostream>
#include <fstream>
#include "audio/AudioFile.h"
#include "assert.h"
#include <list>
#include <map>
using namespace std;
#define assertm(exp, msg) assert(((void)msg, exp))

double* predictor(int arr_size, double* sample){
    double* pred_array = (double*) malloc(arr_size*sizeof(double));
    for(int i = 0; i<arr_size; i++){
        if(i == 0){
            pred_array[i] = 0.0;
        }
        else if(i == 1){
            pred_array[i] = sample[i-1];
        }
        else if(i == 2){
            pred_array[i] = (2 * sample[i-1]) - sample[i-2];
        }
        else{
            pred_array[i] =  (3 * sample[i-1]) - (3*sample[i]-2) + sample[i-3];

        }
    }
    return pred_array;
}

double* residual(int arr_size, double* sample, int arr2_size, double*pred_array){
    assert(arr_size == arr2_size);
    map<char, int> m;
    double* residual = (double*) malloc(arr_size*sizeof(double));
    for(int i = 0; i<arr_size; i++){
        residual[i] = sample[i] - pred_array[i];
        
    }
    return residual;
}

int main(int argc, char *argv[]){
 /*    double lastSample = 0;
    list<double> lista;
    list<double>::iterator it; */
    assertm(argc > 1,"Use ./ex1 <path to the input audio file> <path to the output audio file> ");
    AudioFile<double> audio;
    AudioFile<double> audioOut;
    bool loaded = audio.load(argv[1]);
    ofstream ofs(argv[2]);
    assert(loaded);
    int arr_size = audio.getNumSamplesPerChannel();
    int num_channels = audio.getNumChannels();
    double* sample = (double*) malloc(arr_size*sizeof(double));
    double* pred_array = (double*) malloc(arr_size*sizeof(double));
    double* resi = (double*) malloc(arr_size*sizeof(double));
    cout<<audio.getNumSamplesPerChannel()<<endl;
    
    for (int nSamples = 0; nSamples < audio.getNumSamplesPerChannel(); nSamples++){
        double currenteSample = audio.samples[0][nSamples];
        sample[nSamples] = currenteSample;
        
        
    }
    pred_array = predictor(arr_size,sample);
    resi = residual(arr_size,sample,arr_size,pred_array);



}


